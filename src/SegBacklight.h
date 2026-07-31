#ifndef SEGBACKLIGHT_H
#define SEGBACKLIGHT_H

#include <stdint.h>

/**
 * @brief Abstract backlight control.
 *
 * Implementations drive the GPIO/PWM peripheral behind a display's backlight.
 */
class SegBacklight {
    public:
        virtual ~SegBacklight() = default;

        /**
         * @brief Initialize the backlight output.
         *
         * @param pwm true to prepare PWM/brightness control, false for plain digital on/off
         */
        virtual void init(bool pwm) = 0;

        /**
         * @brief Drive the backlight on/off.
         *
         * @param state true to turn the backlight on, false to turn it off
         */
        virtual void setDigital(bool state) = 0;

        /**
         * @brief Drive the backlight brightness.
         *
         * @param brightness Brightness level 0-255
         */
        virtual void setPWM(uint8_t brightness) = 0;
};

#endif
