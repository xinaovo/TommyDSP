/*
   I2S output example
*/
//#define PIN_I2S_WS 1
#define PIN_I2S_BCK 2 //WS->3
#define PIN_I2S_DOUT 10
#define PIN_I2S_DIN 9
#define SAMPLE_RATE 48000
#include <I2S.h>

I2S i2s(OUTPUT);

int frequency = 440; // frequency of square wave in Hz
const int amplitude = 30000; // amplitude of square wave
const int sampleRate = 48000;

int halfWavelength = (sampleRate / frequency); // half wavelength of square wave

int32_t sample = amplitude; // current sample value
int count = 0;

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

void loop() {

  frequency = millis() %10000;
  halfWavelength = (SAMPLE_RATE / frequency / 2);
  if (count % halfWavelength == 0) {
    // invert the sample every half wavelength count multiple to generate square wave
    sample = -sample;
  }

  i2s.write16(sample, sample);

  // increment the counter for the next sample
  count++;
}
