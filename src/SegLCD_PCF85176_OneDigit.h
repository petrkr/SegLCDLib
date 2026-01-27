#ifndef SEGLCD_PCF85176_ONEDIGIT_H
#define SEGLCD_PCF85176_ONEDIGIT_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @file SegLCD_PCF85176_OneDigit.h
 * @brief PCF85176 driver for single-digit LCD display.
 */

/**
 * @brief Single-digit LCD with flexible segment layout (PCF85176).
 *
 * Up to 5 segments per digit. I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 *
 * @see examples/PCF85176/OneDigit/OneDigit.ino
 */
class SegLCD_PCF85176_OneDigit : public SegDriver_PCF85176 {
    public:
        SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void init(bool reverse);
        void init(bool reverse, bool v1fix);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;
    private:
        static constexpr uint8_t RAM_SIZE = 5;

        static constexpr uint8_t FLAG_REVERSE = 0x01;
        static constexpr uint8_t FLAG_V1FIX = 0x02;

        static constexpr uint8_t DIGITS = 5; // TODO: Do it somehow dynamic as this can be dynamic and this is just maximum

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = DIGITS-1; // All digits supports decimal
        static constexpr int8_t DECIMAL_RAM_OFFSET = -1;  // -1: dot belongs to previous position
};
#endif
