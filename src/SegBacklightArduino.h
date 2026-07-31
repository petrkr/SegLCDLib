#ifndef SEGBACKLIGHTARDUINO_H
#define SEGBACKLIGHTARDUINO_H

#ifndef SEGLCD_DISABLE_ARDUINO_TRANSPORT

#include <Arduino.h>

#include "SegBacklight.h"

/**
 * @brief Arduino GPIO/PWM implementation of backlight control.
 */
class SegBacklightArduino : public SegBacklight {
    public:
        /**
         * @brief Create Arduino backlight control.
         *
         * @param pin Backlight GPIO pin
         * @param activeHigh true if backlight is active high (default), false for active low
         */
        SegBacklightArduino(uint8_t pin, bool activeHigh = true);

        void init(bool pwm) override;
        void setDigital(bool state) override;
        void setPWM(uint8_t brightness) override;

    private:
        uint8_t _pin;
        bool _activeHigh;

        /**
         * @brief LEDC channel for old ESP32 Arduino Core (2.0.x)
         */
        #if defined(ESP32) && ESP_IDF_VERSION_MAJOR < 5
        uint8_t _channel = 0;
        #endif
};

#endif

#endif
