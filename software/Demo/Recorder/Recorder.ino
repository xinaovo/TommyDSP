/**
 * @file Recorder.ino
 * @brief Pitch Shifting Effect Demo
 * @author Jason Chen, Phil Schatzmann
 * @copyright GPLv3
 */

#include "AudioTools.h"
#include "AudioTools/AudioLibs/MemoryManager.h"


// Test setup pinout
#define PIN_BOARD_LED 25
#define PIN_BUTTON 23

#define AUDIO_SAMP_RATE 16000
#define AUDIO_NUMBER_CH 2
#define AUDIO_BITS_PER_SAMPLE 32

AudioInfo info(AUDIO_SAMP_RATE, AUDIO_NUMBER_CH, AUDIO_BITS_PER_SAMPLE);

I2SStream i2s;
MemoryManager memory(500); // Activate SPI RAM for objects > 500 bytes
DynamicMemoryStream recording(false); // Audio stored on heap NOT looping endlessly 

const int pitch_shift_buffer_size = 1000;
const float pitch_shift_ratio = 2; 
NumberFormatConverterStreamT<int16_t, int32_t> out_conv(i2s);
PitchShiftOutput<int16_t, VariableSpeedRingBuffer<int16_t>> pitch_shifter(out_conv);
NumberFormatConverterStreamT<int32_t, int16_t> in_conv(pitch_shifter);

StreamCopy copier; // copies data

uint8_t record_status = false; //0: stop 1: play, 2:record

void record_start(){
  Serial.println("Recording...");
  recording.begin(); // clear data
  record_status = 2;
  copier.begin(recording, i2s); // from i2s mic to memory 
}

void play_start(){
  Serial.println("Playing...");
  record_status = 1;
  copier.begin(in_conv, recording);  // from memory to i2s dac
}

void stop_play(){
  Serial.println("Stop.");
  copier.end();
}

void setup(void) { 
  //initialize LED and button pin
  pinMode(PIN_BOARD_LED, OUTPUT);
  digitalWrite(PIN_BOARD_LED, HIGH);
  pinMode(PIN_BUTTON, INPUT);

  // Open Serial 
  Serial.begin(115200);
  delay(3000); // Wait for serial monitor
  Serial.println("===Recorder Demo===");
  Serial.println("===Begin init===");

  // change to Warning to improve the quality
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning); 

  // convert 32bit to 16bit for input
  in_conv.begin();

  // convert 16bit to 32bit for output
  out_conv.begin();

  // initialize I2S
  auto config = i2s.defaultConfig(RXTX_MODE);
  config.copyFrom(info); 
  config.i2s_format = I2S_STD_FORMAT;
  config.pin_ws = 5;
  config.pin_bck = 6;
  config.pin_data = 3;
  config.pin_data_rx = 4;
  //config.fixed_mclk = sample_rate * 256;
  //config.pin_mck = 7; // must be 0,1 or 3 - only for ESP_IDF_VERSION_MAJOR >= 4
  i2s.begin(config);

  // initialize pitch shifter
  auto pcfg = pitch_shifter.defaultConfig();
  pcfg.copyFrom(info);
  pcfg.bits_per_sample = 16;
  pcfg.pitch_shift = pitch_shift_ratio;
  pcfg.buffer_size = pitch_shift_buffer_size;
  pitch_shifter.begin(pcfg);

  copier.begin();
  digitalWrite(PIN_BOARD_LED, LOW);
  Serial.println("===End init===");

}

unsigned long last_time = millis();
bool last_button_status = digitalRead(PIN_BUTTON);
bool current_button_status;
void loop() {
  if(millis() - last_time > 100){
    current_button_status = digitalRead(PIN_BUTTON);
    if(current_button_status != last_button_status){
      if(current_button_status == LOW){ // Button pressed
        record_start();
        digitalWrite(PIN_BOARD_LED, HIGH);
      }else{
        play_start();
        digitalWrite(PIN_BOARD_LED, LOW);
      }
    }

    last_button_status = digitalRead(PIN_BUTTON);
    last_time = millis();
  }
  copier.copy();
}
