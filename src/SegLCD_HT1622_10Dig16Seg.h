#ifndef SEGLCD_HT1622_10Dig16Seg_H
#define SEGLCD_HT1622_10Dig16Seg_H

#include <SegDriver_HT1622.h>


class SegLCD_HT1622_10Dig16Seg : public SegDriver_HT1622 {
    public:
        SegLCD_HT1622_10Dig16Seg(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void clear() override;
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void writeDigit16seg(uint8_t row, uint8_t col, char c);
        size_t write(uint8_t ch) override;

    private:
        uint8_t _buffer[48] = {0};  // VK0192 RAM buffer

        uint16_t _map16Segments(uint16_t val);
        int8_t _get16SegmentsAddress(uint8_t row, uint8_t col);

};
#endif
