#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "input.h"

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
const int TEXT_SIZE = 2;
const int CHAR_HEIGHT = 16;
const int DISPLAY_HEIGHT = 240;
const int MAX_LINES = DISPLAY_HEIGHT / CHAR_HEIGHT;
const int DISPLAY_WIDTH = 240;
const int CHAR_WIDTH = 6 * TEXT_SIZE;
const int LINE_WIDTH = (DISPLAY_WIDTH - 8) / CHAR_WIDTH;
String lineBuffer[15];
int currentLine = 0;
const int LINE_DELAY = 1500;

void scrollDisplay(String newLine) {
    for(int i = 0; i < MAX_LINES - 1; i++) {
        lineBuffer[i] = lineBuffer[i + 1];
    }
    lineBuffer[MAX_LINES - 1] = newLine;
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(TEXT_SIZE);
    for(int i = 0; i < MAX_LINES; i++) {
        tft.setCursor(4, i * CHAR_HEIGHT);
        tft.print(lineBuffer[i]);
    }
}

void displayText(const char* text) {
    for(int i = 0; i < MAX_LINES; i++) {
        lineBuffer[i] = "";
    }
    String line = "";
    int len = strlen(text);
    for(int i = 0; i < len; i++) {
        char c = text[i];
        if(c == '\n' || c == '\r') {
            while(line.length() > 0) {
                int segmentLen = min((int)line.length(), LINE_WIDTH);
                String segment = line.substring(0, segmentLen);
                scrollDisplay(segment);
                line = line.substring(segmentLen);
            }
            delay(LINE_DELAY);
        }
        else {
            line += c;
            if(line.length() >= LINE_WIDTH) {
                String segment = line.substring(0, LINE_WIDTH);
                scrollDisplay(segment);
                line = line.substring(LINE_WIDTH);
            }
        }
    }
    while(line.length() > 0) {
        int segmentLen = min((int)line.length(), LINE_WIDTH);
        String segment = line.substring(0, segmentLen);
        scrollDisplay(segment);
        line = line.substring(segmentLen);
    }
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
    displayText(inputText);
}

void loop() {
}
