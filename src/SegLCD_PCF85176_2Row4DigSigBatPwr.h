#ifndef SEGLCD_PCx85_2Row4DigSigBatPwr_H
#define SEGLCD_PCx85_2Row4DigSigBatPwr_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @file SegLCD_PCF85176_2Row4DigSigBatPwr.h
 * @brief PCF85176 driver for 2-row 4-digit LCD with signal/battery/power.
 *
 * I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 */
class SegLCD_PCF85176_2Row4DigSigBatPwr : public SegDriver_PCF85176 {
    public:
        SegLCD_PCF85176_2Row4DigSigBatPwr(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setPowerSymbol(bool state, uint8_t index = 0);
        size_t write(uint8_t ch) override;

    private:
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        bool _handleDotNoPrevClear(uint8_t ch);
        static constexpr uint8_t ADDR_POWER = 6;
        static constexpr uint8_t ADDR_SIGNAL = 14;
        static constexpr uint8_t ADDR_BATT = 16;
        static constexpr uint8_t ADDR_1_SEGS = 0;
        static constexpr uint8_t ADDR_2_SEGS = 8;

        static constexpr uint8_t RAM_SIZE = 9;

        // RAM layout (matches _ramBuffer allocation):
        static constexpr uint8_t OFFSET_POWER = ADDR_POWER / 2;
        static constexpr uint8_t OFFSET_SIGNAL = ADDR_SIGNAL / 2;
        static constexpr uint8_t OFFSET_BATT = ADDR_BATT / 2;
        static constexpr uint8_t OFFSET_ROW1 = ADDR_1_SEGS / 2;
        static constexpr uint8_t OFFSET_ROW2 = ADDR_2_SEGS / 2;

        static constexpr uint8_t ROW1 = 0;
        static constexpr uint8_t ROW2 = 1;

        static constexpr uint8_t DECIMAL_POINT_BIT_ROW1 = 0x01;
        static constexpr uint8_t DECIMAL_POINT_BIT_ROW2 = 0x08;
        static constexpr uint8_t DECIMAL_MIN_ROW = 0;
        static constexpr uint8_t DECIMAL_MAX_ROW = 1;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 3;
        static constexpr int8_t DECIMAL_COL_OFFSET = -1;

        // Signal/Battery level constants
        static constexpr uint8_t MAX_BATTERY_LEVEL = 4;
        static constexpr uint8_t BATTERY_MASK = 0xF0;
        static constexpr uint8_t BATTERY_LEVEL_SEG[MAX_BATTERY_LEVEL] = {0x80, 0x20, 0x40, 0x10};

        static constexpr uint8_t MAX_SIGNAL_LEVEL = 1;
        static constexpr uint8_t SIGNAL_MASK = DECIMAL_POINT_BIT_ROW2;
        static constexpr uint8_t SIGNAL_LEVEL_BITS[MAX_SIGNAL_LEVEL] = {DECIMAL_POINT_BIT_ROW2};

        static constexpr uint8_t POWER_BIT = DECIMAL_POINT_BIT_ROW1;

        uint8_t _mapSegmentsRow1(uint8_t val);
        uint8_t _mapSegmentsRow2(uint8_t val);
};
#endif
