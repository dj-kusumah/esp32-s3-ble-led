#ifndef RGB_LED_H
#define RGB_LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class RgbLed {
public:
    RgbLed(uint8_t pin);
    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setBrightness(uint8_t brightness);
    void on();
    void off();
    void toggle();
    void red();
    void green();
    void blue();
    void white();
    void yellow();
    void cyan();
    void magenta();
    bool isOn();

private:
    Adafruit_NeoPixel _pixel;
    uint8_t _pin;
    bool _state;
    uint8_t _r, _g, _b;
};

#endif
