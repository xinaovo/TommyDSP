#include "AudioTools.h"
#include "AudioTools/AudioLibs/MemoryManager.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

//#define BSP_I2S_MCLK 7
#define BSP_I2S_BCLK 6
#define BSP_I2S_WS 5
#define BSP_I2S_DOUT 3
#define BSP_I2S_DIN 4

#define BSP_TFT_CS 9
#define BSP_TFT_DC 13
#define BSP_TFT_MOSI 11
#define BSP_TFT_SCK 10
#define BSP_TFT_RST 12
#define BSP_TFT_BL 8

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

Adafruit_ST7735 tft = Adafruit_ST7735(&SPI1, BSP_TFT_CS, BSP_TFT_DC, BSP_TFT_RST);

void record_start(){
  Serial.println("Recording...");
  recording.begin(); // clear data
  copier.begin(recording, i2s); // from i2s mic to memory 
}

void play_start(){
  Serial.println("Playing...");
  copier.begin(in_conv, recording);  // from memory to i2s dac
}

// void stop_play(){
//   Serial.println("Stop.");
//   copier.end();
// }

void setup(void) { 
  //initialize LED and button pin
  pinMode(PIN_BOARD_LED, OUTPUT);
  digitalWrite(PIN_BOARD_LED, HIGH);
  pinMode(PIN_BUTTON, INPUT);

  // Open Serial 
  Serial.begin(115200);
  delay(3000); // Wait for serial monitor
  Serial.println("===PitchShift Recorder ===");
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
  config.pin_bck = BSP_I2S_BCLK;
  config.pin_data = BSP_I2S_DOUT;
  config.pin_data_rx = BSP_I2S_DIN;
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

void setup1(){
  SPI1.setTX(BSP_TFT_MOSI);
  SPI1.setSCK(BSP_TFT_SCK);
  SPI1.setCS(BSP_TFT_CS);
  SPI1.setRX(NOPIN);
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  pinMode(BSP_TFT_BL, OUTPUT);
  digitalWrite(BSP_TFT_BL, HIGH); // Turn on LCD Backlight
}

// Loop on Core 0
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


void draw_animation(){
  tft.fillScreen(ST77XX_RED);
    for (int i = 0; i < 5; i++) {
      tft.drawCircle(63, 79, 35 + (int) (pow(sin(millis() / 1e3 * 6.28 * 1.0 + i * 0.15) * 0.5 + 0.5, 3.0) * 15.0), ST77XX_BLACK);
    }
}

//Loop on Core 1(Graphics)
void loop1(){
  draw_animation();
}
