#ifndef SEGLCD_DISABLE_ARDUINO_TRANSPORT

#include <Arduino.h>
#include <SegBacklightArduino.h>

SegBacklightArduino::SegBacklightArduino(uint8_t pin, bool activeHigh) : _pin(pin), _activeHigh(activeHigh) {}

void SegBacklightArduino::init(bool pwm) {
    if (pwm) {
        #ifdef ESP32
            #if ESP_IDF_VERSION_MAJOR >= 5
                // ESP32 PWM setup - modern API (3.3+, IDF 5.x)
                ledcAttach(_pin, 5000, 8);  // pin, frequency, resolution
            #else
                // Old API (2.0.x, IDF 4.x)
                _channel = 0;
                ledcSetup(_channel, 5000, 8);
                ledcAttachPin(_pin, _channel);
            #endif
        #else
            pinMode(_pin, OUTPUT);
        #endif
    } else {
        pinMode(_pin, OUTPUT);
    }
}

void SegBacklightArduino::setDigital(bool state) {
    bool outputState = _activeHigh ? state : !state;
    digitalWrite(_pin, outputState ? HIGH : LOW);
}

void SegBacklightArduino::setBrightness(uint8_t brightness) {
    uint8_t pwmValue = brightness;
    if (!_activeHigh) {
        pwmValue = 255 - brightness;
    }

    #ifdef ESP32
        #if ESP_IDF_VERSION_MAJOR >= 5
            ledcWrite(_pin, pwmValue);
        #else
            ledcWrite(_channel, pwmValue);
        #endif
    #else
        analogWrite(_pin, pwmValue);
    #endif
}

#endif
