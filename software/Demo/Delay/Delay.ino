/**
 * @file Delay.ino
 * @brief Delay Effect Demo
 * @author Jason Chen, Phil Schatzmann
 * @copyright GPLv3
 */

#include "AudioTools.h"
 
AudioInfo info(48000, 2, 32);

I2SStream i2s;
NumberFormatConverterStreamT<int16_t, int32_t> out_conv(i2s);
AudioEffectStream effects(out_conv);
NumberFormatConverterStreamT<int32_t, int16_t> in_conv(effects);
StreamCopy copier(in_conv, i2s); 

Delay dly(998, 0.5, 0.0,info.sample_rate);

void setup(void) { 
  // Open Serial 
  Serial.begin(115200);
  delay(3000); // Wait for serial monitor
  Serial.println("===Begin init===");

  // change to Warning to improve the quality
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning); 

  // convert 32bit to 16bit for input
  in_conv.begin();

  // convert 16bit to 32bit for output
  out_conv.begin();

  // start I2S
  auto config = i2s.defaultConfig(RXTX_MODE);
  config.copyFrom(info); 
  config.i2s_format = I2S_STD_FORMAT;
  config.pin_ws = 5;
  config.pin_bck = 6;
  config.pin_data = 3;
  config.pin_data_rx = 4;
  //config.fixed_mclk = sample_rate * 256;
  //config.pin_mck = 7; // must be 0,1 or 3 - only for ESP_IDF_VERSION_MAJOR >= 4
  
  
  auto delay_cfg = effects.defaultConfig();
  delay_cfg.copyFrom(info);
  delay_cfg.bits_per_sample = 16;

  effects.addEffect(dly);

  i2s.begin(config);
  effects.begin(delay_cfg);
  copier.begin();

  Serial.println("===End init===");
}

// Arduino loop - copy sound to out 
void loop() {
  copier.copy();
}
