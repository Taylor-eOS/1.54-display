#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#define SCLK_PIN 4
#define MOSI_PIN 6
#define MISO_PIN -1
#define CS_PIN 7
#define DC_PIN 8
#define RST_PIN 9
#define BL_PIN 21
Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, CS_PIN, DC_PIN, RST_PIN);
const int BL_Channel = 0;
const int BL_Freq = 5000;
const int BL_Resolution = 8;

void drawDiagnostics() {
    tft.drawRect(4, 40, 232, 196, ST77XX_WHITE);
    tft.drawLine(4, 40, 236, 236, ST77XX_YELLOW);
    tft.drawLine(236, 40, 4, 236, ST77XX_CYAN);
    tft.setTextSize(1);
    tft.setCursor(10, 220);
    tft.print("Rotation 0 240x240");
}

void setup() {
    Serial.begin(115200);
    SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
    pinMode(BL_PIN, OUTPUT);
    ledcSetup(BL_Channel, BL_Freq, BL_Resolution);
    ledcAttachPin(BL_PIN, BL_Channel);
    ledcWrite(BL_Channel, 255);
    tft.init(240, 240);
    tft.setRotation(0);
    tft.fillScreen(ST77XX_BLACK);
    delay(100);
    tft.fillScreen(ST77XX_RED);
    delay(300);
    tft.fillScreen(ST77XX_GREEN);
    delay(300);
    tft.fillScreen(ST77XX_BLUE);
    delay(300);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(8, 8);
    tft.print("ST7789 1.54\" Test");
    drawDiagnostics();
}

void loop() {
    for (int b = 0; b <= 255; b += 5) {
        ledcWrite(BL_Channel, b);
        delay(20);
    }
    for (int b = 255; b >= 0; b -= 5) {
        ledcWrite(BL_Channel, b);
        delay(20);
    }
    tft.invertDisplay(true);
    delay(400);
    tft.invertDisplay(false);
    delay(400);
}

