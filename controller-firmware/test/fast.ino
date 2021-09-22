#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

//Display connections:
#define TFT_CS  14
#define TFT_RST 33
#define TFT_DC  27
//      TFT_SDA 23
//      TFT_SCL 18

Adafruit_ST7735 tft = Adafruit_ST7735(&SPI, TFT_CS, TFT_DC, TFT_RST);   //-Just used for setup

class  aFrameBuffer : public Adafruit_GFX {
  public:
    uint16_t *buffer;
    aFrameBuffer(int16_t w, int16_t h): Adafruit_GFX(w, h)
    {
      buffer = (uint16_t*)malloc(2 * h * w);
      for (int i = 0; i < h * w; i++)
        buffer[i] = 0;

      tft.initR();
      tft.setRotation(1);
      tft.fillScreen(ST77XX_BLACK);
    }
    void drawPixel( int16_t x, int16_t y, uint16_t color)
    {
      if (x > 159)
        return;
      if (x < 0)
        return;
      if (y > 127)
        return;
      if (y < 0)
        return;
      buffer[x + y * _width] = color;
    }

    void display()
    {
      tft.setAddrWindow(0, 0, 160, 128);
      digitalWrite(TFT_DC, HIGH);
      digitalWrite(TFT_CS, LOW);
      SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
      for (uint16_t i = 0; i < 160 * 128; i++)
      {
        SPI.write16(buffer[i]);
      }
      SPI.endTransaction();
      digitalWrite(TFT_CS, HIGH);
    }
};

aFrameBuffer frame(160, 128);

int cnt = 0;
int cnt0 = 0;
int rate = 0;

unsigned long timer;

void setup() {
  timer = millis() + 1000;
}

void loop() {
  if (millis() >= timer)
  {
    timer += 1000;
    rate=cnt-cnt0;
    cnt0=cnt;
  }
  for (int y = 0; y < 128; y++)
    for (int x = 0; x < 160; x++)
      frame.drawPixel(x, y, random(65535));
  frame.fillRect(38, 58, 100, 15, 0);
  frame.setCursor(40, 60);
  frame.print("Frame:");
  frame.print(cnt++);
  frame.print("/");
  frame.print(rate);

  frame.display();
}
