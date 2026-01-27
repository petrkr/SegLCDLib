#ifndef SEGLCD_HT1622_10Dig16Seg_H
#define SEGLCD_HT1622_10Dig16Seg_H

#include <SegDriver_HT1622.h>

/**
 * @file SegLCD_HT1622_10Dig16Seg.h
 * @brief HT1622 driver for 10-digit 16-segment LCD.
 */

/**
 * @brief 10-digit 16-segment alphanumeric LCD (HT1622).
 *
 * Features: decimal points, full alphanumeric characters.
 * Integrated HT1622 controller, 3-wire serial protocol.
 * Strict timing requirements (4us pulse width).
 *
 * @see examples/HT1622/10Digit16SegmentLCD/10Digit16SegmentLCD.ino
 */
class SegLCD_HT1622_10Dig16Seg : public SegDriver_HT1622 {
    public:
        SegLCD_HT1622_10Dig16Seg(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void writeDigit16seg(uint8_t row, uint8_t col, char c);
        size_t write(uint8_t ch) override;

    private:
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t RAM_SIZE = 23;  // 10 digits * 2 bytes + 3 bytes for decimals = ~23 bytes
        static constexpr uint8_t DIGITS = 10;

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 8;
        static constexpr int8_t DECIMAL_RAM_OFFSET = -1;

        uint16_t _map16Segments(uint16_t val);
        int8_t _get16SegmentsAddress(uint8_t row, uint8_t col);

};
#endif
