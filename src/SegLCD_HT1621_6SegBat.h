#ifndef SEGLCD_HT1621_6SEGBAT_H
#define SEGLCD_HT1621_6SEGBAT_H

#include <SegDriver_HT1621.h>

/**
 * @brief HT1621 driver for 6-digit LCD with battery indicator
 *
 * 6-digit 7-segment display with integrated HT1621 controller.
 * Features: battery level indicator, decimal points.
 *
 * @note Integrated controller on LCD module (no separate IC)
 * @note 3-wire serial protocol (CLK, DATA, CS pins)
 *
 * @example examples/HT1621/6DigBat/6DigBat.ino
 */
class SegLCD_HT1621_6SegBat : public SegDriver_HT1621 {
    #define ADDR_BATT 6
    public:
        SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void clear();
        void setBatteryLevel(uint8_t level);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;

    private:
        static constexpr uint8_t RAM_SIZE = 6;

        // Note: FLAG_PENDING_DOT (0x80) from SegLCDLib.h is used for decimal handling

        static constexpr uint8_t DIGITS = 6;

        // Decimal configuration: RAM offset 0 (dot in same byte, needs FLAG_PENDING_DOT)
        static constexpr uint8_t DECIMAL_MIN_COL = 3;
        static constexpr uint8_t DECIMAL_MAX_COL = 5;
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x80;
        static constexpr int8_t DECIMAL_RAM_OFFSET = 0;   // 0: dot in same byte

        // Signal/Battery level constants
        static constexpr uint8_t MAX_BATTERY_LEVEL = 3;
        static constexpr uint8_t BATTERY_MASK = 0x80;
        static constexpr uint8_t BATTERY_LEVEL_SEG[3] = {2, 1, 0};

        uint8_t _mapSegments(uint8_t val);
};
#endif
