#ifndef SEGLCD_HT1622_10Dig16Seg_H
#define SEGLCD_HT1622_10Dig16Seg_H

#include <SegDriver_HT1622.h>

/**
 * @brief HT1622 driver for 10-digit 16-segment LCD
 *
 * 10-digit display with 16-segment capability per digit (alphanumeric).
 * Integrated HT1622 controller with enhanced RAM.
 * Features: decimal points, 16-segment characters.
 *
 * @note Integrated controller on LCD module (no separate IC)
 * @note 3-wire serial protocol (CLK, DATA, CS pins)
 * @note Strict timing requirements (4μs pulse width)
 *
 * @example examples/HT1622/10Digit16SegmentLCD/10Digit16SegmentLCD.ino
 */
class SegLCD_HT1622_10Dig16Seg : public SegDriver_HT1622 {
    public:
        SegLCD_HT1622_10Dig16Seg(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void clear() override;
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void writeDigit16seg(uint8_t row, uint8_t col, char c);
        size_t write(uint8_t ch) override;

    private:
        static constexpr uint8_t RAM_SIZE = 45;
        static constexpr uint8_t DIGITS = 10;

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 8;
        static constexpr int8_t DECIMAL_RAM_OFFSET = -1;

        uint16_t _map16Segments(uint16_t val);
        int8_t _get16SegmentsAddress(uint8_t row, uint8_t col);

};
#endif
