#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
public:
    Led(uint8_t pin);
    void begin();
    void on();
    void off();
    void toggle();
    bool isOn();

private:
    uint8_t _pin;
    bool _state;
};

#endif
