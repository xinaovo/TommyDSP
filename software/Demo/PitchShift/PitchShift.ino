/**
 * @file PitShift.ino
 * @brief Pitch Shifting Effect Demo
 * @author Jason Chen, Phil Schatzmann
 * @copyright GPLv3
 */

#include "AudioTools.h"

const int pitch_shift_buffer_size = 5000;
const float pitch_shift_ratio = 2; 
AudioInfo info(48000, 2, 32);

I2SStream i2s;
NumberFormatConverterStreamT<int16_t, int32_t> out_conv(i2s);
PitchShiftOutput<int16_t, VariableSpeedRingBuffer<int16_t>> pitch_shifter(out_conv);
NumberFormatConverterStreamT<int32_t, int16_t> in_conv(pitch_shifter);
StreamCopy copier(in_conv, i2s); 

void setup(void) { 
  // Open Serial 
  Serial.begin(115200);
  delay(3000); // Wait for serial monitor
  Serial.println("===Pitch Shift Demo===");
  Serial.println("===Begin init===");

  // change to Warning to improve the quality
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Error); 

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

  Serial.println("===End init===");
}

// Arduino loop - copy sound to out 
void loop() {
  copier.copy();
}
