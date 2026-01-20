#ifndef SEGLCD_PCF85176_ONEDIGIT_H
#define SEGLCD_PCF85176_ONEDIGIT_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @brief PCF85176 driver for single-digit LCD display
 *
 * Single custom segment display (up to 5 segments).
 * Flexible segment layout for custom applications.
 *
 * @note Separate PCF85176 I2C controller (not integrated)
 * @note I2C protocol (SDA, SCL pins)
 * @note Default I2C address: 0x38
 *
 * @example examples/PCF85176/OneDigit/OneDigit.ino
 */
class SegLCD_PCF85176_OneDigit : public SegDriver_PCF85176 {

    #define MAX_ADDRESS 39

    public:
        SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void init(bool reverse);
        void init(bool reverse, bool v1fix);
        void clear() override;
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;
    private:
        uint8_t _buffer[MAX_ADDRESS/8];

        static constexpr uint8_t FLAG_REVERSE = 0x01;
        static constexpr uint8_t FLAG_V1FIX = 0x02;

        static constexpr uint8_t DIGITS = 5; // TODO: Do it somehow dynamic as this can be dynamic and this is just maximum
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = DIGITS-1; // All digits supports decimal
};
#endif
