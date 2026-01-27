#ifndef SEGLCD_HT1621_6SEGBAT_H
#define SEGLCD_HT1621_6SEGBAT_H

#include <SegDriver_HT1621.h>

/**
 * @file SegLCD_HT1621_6SegBat.h
 * @brief HT1621 driver for 6-digit LCD with battery indicator.
 */

/**
 * @brief 6-digit 7-segment LCD with battery indicator (HT1621).
 *
 * Features: battery level indicator, decimal points.
 * Integrated HT1621 controller, 3-wire serial protocol.
 *
 * @see examples/HT1621/6DigBat/6DigBat.ino
 */
class SegLCD_HT1621_6SegBat : public SegDriver_HT1621 {
    public:
        SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        size_t write(uint8_t ch) override;

    private:
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t ADDR_BATT = 6;
        static constexpr uint8_t RAM_SIZE = 6;
        static constexpr uint8_t DIGITS = 6;

        // Decimal configuration (physical digits 2,3,4 have decimal points)
        static constexpr uint8_t DECIMAL_MIN_COL = 2;
        static constexpr uint8_t DECIMAL_MAX_COL = 4;
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x80;
        static constexpr int8_t DECIMAL_RAM_OFFSET = 0;   // 0: dot in same byte

        // Signal/Battery level constants
        static constexpr uint8_t MAX_BATTERY_LEVEL = 3;
        static constexpr uint8_t BATTERY_MASK = 0x80;
        static constexpr uint8_t BATTERY_LEVEL_SEG[3] = {2, 1, 0};

        uint8_t _mapSegments(uint8_t val);
};
#endif
