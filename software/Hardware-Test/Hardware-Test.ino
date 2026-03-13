#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define PIN_I2C_SCL 1
#define PIN_I2C_SDA 2

#define PIN_I2S_MCLK 7
#define PIN_I2S_BCLK 6
#define PIN_I2S_LRCK 5
#define PIN_I2S_DOUT 3
#define PIN_I2S_DIN 4

#define PIN_TFT_CS 9
#define PIN_TFT_DC 13
#define PIN_TFT_MOSI 11
#define PIN_TFT_SCK 10
#define PIN_TFT_RST 12
#define PIN_TFT_BL 8

#define PIN_SD_CLK_SCK 18
#define PIN_SD_CMD_MOSI 19
#define PIN_SD_DAT0_MISO 20
#define PIN_SD_DAT1 21
#define PIN_SD_DAT2 22
#define PIN_SD_DAT3_CS 23
#define PIN_SD_DET 24

#define PIN_PEDAL_1 14
#define PIN_PEDAL_2 15

#define PIN_ROTARY_ENCODER_CLK 16
#define PIN_ROTARY_ENCODER_DT 17
#define PIN_ROTARY_ENCODER_BUTTON 25

#define PIN_VBAT 26
#define PIN_KNOB_1 27
#define PIN_KNOB_2 28
#define PIN_KNOB_3 29

Adafruit_ST7735 tft = Adafruit_ST7735(&SPI1, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(PIN_PEDAL_1, INPUT);

  SPI1.setTX(PIN_TFT_MOSI);
  SPI1.setSCK(PIN_TFT_SCK);
  SPI1.setCS(PIN_TFT_CS);
  SPI1.setRX(NOPIN);
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  pinMode(PIN_TFT_BL, OUTPUT);
  digitalWrite(PIN_TFT_BL, HIGH); // Turn on LCD Backlight
  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd_test();
  // Serial.print("VBAT: ");
  // Serial.println(analogRead(PIN_VBAT), DEC);
  // Serial.print("KNOB1: ");
  // Serial.println(analogRead(PIN_KNOB_1), DEC);
  // Serial.print("KNOB2: ");
  // Serial.println(analogRead(PIN_KNOB_2), DEC);
  // Serial.print("KNOB3: ");
  // Serial.println(analogRead(PIN_KNOB_3), DEC);
  // delay(1000);
}

// LCD Test functions from AdafruitGFX examples
uint32_t i = 0;
void lcd_test(void){
  // tft print function!
  if(i % 100 == 0){
    tftPrintTest();
    delay(2000);
  }
  
  const uint16_t delay_time = 1;

  tft.fillScreen(ST77XX_BLACK);
  delay(delay_time);
  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE);
  delay(delay_time);

  // a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);
  delay(delay_time);

  // line draw test
  testlines(ST77XX_YELLOW);
  delay(delay_time);

  // optimized lines
  testfastlines(ST77XX_RED, ST77XX_BLUE);
  delay(delay_time);

  testdrawrects(ST77XX_GREEN);
  delay(delay_time);

  testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
  delay(delay_time);

  tft.fillScreen(ST77XX_BLACK);
  testfillcircles(10, ST77XX_BLUE);
  testdrawcircles(10, ST77XX_WHITE);
  delay(delay_time);

  testroundrects();
  delay(delay_time);

  testtriangles();
  delay(delay_time);

  i += 1;

}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  const float p = 3.1415926;
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}