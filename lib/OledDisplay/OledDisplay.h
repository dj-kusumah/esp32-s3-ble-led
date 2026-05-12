#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OledDisplay {
public:
    OledDisplay(uint8_t width = 128, uint8_t height = 64, uint8_t address = 0x3C);
    bool begin(int sdaPin = 8, int sclPin = 9);

    // Tampilkan status lengkap
    void showStatus(const char* mode, const char* ledLuar,
                    const char* ledRgb, const char* bleStatus);
    void showMessage(const char* line1, const char* line2 = nullptr);
    void clear();
    Adafruit_SSD1306& getDisplay();

private:
    Adafruit_SSD1306 _display;
    uint8_t _address, _width, _height;
};

#endif
