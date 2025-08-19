#ifndef SEGLCD_PCF85176_4DR821B_H
#define SEGLCD_PCF85176_4DR821B_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_4DR821B : public SegDriver_PCF85176 {

    #define MAX_ADDRESS 38

    public:
        SegLCD_PCF85176_4DR821B(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void clear() override;
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;
    private:
        uint8_t _buffer[5];
        bool _previousDot = false;

        static constexpr uint8_t ADDR_SYMBOLS = 0x00;
        static constexpr uint8_t ADDR_SEGS = 0x01;
        static constexpr uint8_t DIGITS = 4;
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 2;
};
#endif
