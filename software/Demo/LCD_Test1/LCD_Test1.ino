/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  The 1.14" TFT breakout
  ----> https://www.adafruit.com/product/4383
  The 1.3" TFT breakout
  ----> https://www.adafruit.com/product/4313
  The 1.54" TFT breakout
    ----> https://www.adafruit.com/product/3787
  The 1.69" TFT breakout
    ----> https://www.adafruit.com/product/5206
  The 2.0" TFT breakout
    ----> https://www.adafruit.com/product/4311
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/
#define TFT_CS        9
#define TFT_DC         13
#define TFT_MOSI       11
#define TFT_SCK        10
#define TFT_RST        12
#define TFT_BL         8

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h>
#include <SPI.h>



// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

// For 1.44" and 1.8" TFT with ST7735 use:
//Adafruit_ST7735 tft = Adafruit_ST7735(&SPI1, TFT_CS, TFT_DC, TFT_RST);
Adafruit_ST7789 tft = Adafruit_ST7789(&SPI1, TFT_CS, TFT_DC, TFT_RST);

void setup(void) {

  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));

  SPI1.setTX(TFT_MOSI);
  SPI1.setSCK(TFT_SCK);
  SPI1.setCS(TFT_CS);
  SPI1.setRX(NOPIN);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  // Use this initializer if using a 1.8" TFT screen:
  //tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.init(240, 240);
}

void loop() {

  draw_animation();
  // delay(1000);
  // draw_animation(false);
  // delay(1000);
}

void draw_animation(){
  tft.fillScreen(ST77XX_BLACK);
  // if(mouth_status == true){
    // for(uint8_t )
    for (int i = 0; i < 5; i++) {
      tft.drawCircle(63, 79, 35 + (int) (pow(sin(millis() / 1e3 * 6.28 * 1.0 + i * 0.15) * 0.5 + 0.5, 3.0) * 15.0), ST77XX_MAGENTA);
    }
  // }else{
    // tft.drawCircle(63, 79, 5, ST77XX_MAGENTA);
  // }
}
