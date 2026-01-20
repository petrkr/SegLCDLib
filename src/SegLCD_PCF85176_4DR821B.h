#ifndef SEGLCD_PCF85176_4DR821B_H
#define SEGLCD_PCF85176_4DR821B_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @brief PCF85176 driver for Tesla 4DR821B / 4DT821B dashboard LCD
 *
 * Tesla dashboard cluster display with custom segment layout.
 * Multiple symbol zones for dashboard indicators and status.
 *
 * @note Separate PCF85176 I2C controller (not integrated)
 * @note I2C protocol (SDA, SCL pins)
 * @note Default I2C address: 0x38
 *
 * @example examples/PCF85176/4DR821B/4DR821B.ino
 */
class SegLCD_PCF85176_4DR821B : public SegDriver_PCF85176 {

    #define MAX_ADDRESS 38

    public:
        typedef enum {
            SYMBOL_ARROW = 0x10,
            SYMBOL_TILDA = 0x80
        } SymbolFlags;

        SegLCD_PCF85176_4DR821B(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void clear() override;
        void setSymbol(uint8_t symbol, bool state);
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;
    private:
        uint8_t _buffer[5];
        bool _previousDot = false;

        static constexpr uint8_t FLAG_COLON_DISPLAYED = 0x01;
        static constexpr uint8_t FLAG_COL0_OVERLAY = 0x02;

        static constexpr uint8_t ADDR_SYMBOLS = 0x00;
        static constexpr uint8_t ADDR_SEGS = 0x01;
        static constexpr uint8_t DIGITS = 4;
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr uint8_t MIDDLE_COLON_BIT = 0x01;
        static constexpr uint8_t LEFT_COLON_BIT = 0x20;
        static constexpr uint8_t MINUS_BIT = 0x40;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 2;

        bool _handleCol0Overlay(uint8_t ch);
};
#endif
