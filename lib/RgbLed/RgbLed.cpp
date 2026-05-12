#include "RgbLed.h"

RgbLed::RgbLed(uint8_t pin)
    : _pixel(1, pin, NEO_GRB + NEO_KHZ800),
      _pin(pin), _state(false), _r(255), _g(255), _b(255) {}

void RgbLed::begin() {
    _pixel.begin();
    _pixel.setBrightness(50);
    off();
}

void RgbLed::setBrightness(uint8_t brightness) {
    _pixel.setBrightness(brightness);
    if (_state) { _pixel.setPixelColor(0, _pixel.Color(_r, _g, _b)); _pixel.show(); }
}

void RgbLed::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _r = r; _g = g; _b = b;
    if (_state) { _pixel.setPixelColor(0, _pixel.Color(_r, _g, _b)); _pixel.show(); }
}

void RgbLed::on()  { _state = true;  _pixel.setPixelColor(0, _pixel.Color(_r, _g, _b)); _pixel.show(); }
void RgbLed::off() { _state = false; _pixel.setPixelColor(0, 0); _pixel.show(); }
void RgbLed::toggle() { if (_state) off(); else on(); }
void RgbLed::red()     { _r=255; _g=0;   _b=0;   on(); }
void RgbLed::green()   { _r=0;   _g=255; _b=0;   on(); }
void RgbLed::blue()    { _r=0;   _g=0;   _b=255; on(); }
void RgbLed::white()   { _r=255; _g=255; _b=255; on(); }
void RgbLed::yellow()  { _r=255; _g=255; _b=0;   on(); }
void RgbLed::cyan()    { _r=0;   _g=255; _b=255; on(); }
void RgbLed::magenta() { _r=255; _g=0;   _b=255; on(); }
bool RgbLed::isOn() { return _state; }
