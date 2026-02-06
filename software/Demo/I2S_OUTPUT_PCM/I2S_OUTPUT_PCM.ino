/*
   I2S output example
*/
//#define PIN_I2S_WS 1
#define PIN_I2S_BCK 2 //WS->3
#define PIN_I2S_DOUT 10
#define PIN_I2S_DIN 9
#include <I2S.h>
#include "music.h"

I2S i2s(OUTPUT);

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("BEGIN INIT");
  i2s.setDOUT(PIN_I2S_DOUT);
 // i2s.setDIN(PIN_I2S_DIN);
  i2s.setBCLK(PIN_I2S_BCK); // Note: LRCLK = BCLK + 1
  i2s.setBitsPerSample(16);
  i2s.setFrequency(SAMPLE_RATE);

  i2s.begin();
}

uint32_t i= 0;
void loop() {

  if(i >= SAMPLE_TOTAL_COUNT){
    i = 0;
  }else{
    i2s.write16(SAMPLES[i],SAMPLES[i + 1]);
    i += 2;
  }

  
}
