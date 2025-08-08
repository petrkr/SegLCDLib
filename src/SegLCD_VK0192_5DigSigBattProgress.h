#ifndef SEGLCD_VT0192_5DigSigBattProgress_H
#define SEGLCD_VT0192_5DigSigBattProgress_H

#include <SegDriver_VK0192.h>


class SegLCD_VK0192_5DigSigBattProgress : public SegDriver_VK0192 {
    public:
        SegLCD_VK0192_5DigSigBattProgress(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void clear() override;
        void setCursor(uint8_t row, uint8_t col) override;
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;

        void writeDigit7seg(uint8_t row, uint8_t col, char c);
        
        // TODO: Remove after display implementation
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);

    private:
        uint8_t _buffer[48] = {0};  // VK0192 RAM buffer
        
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x10;
        static constexpr uint8_t NUM_7SEG_DIGITS = 10;  // 3+2+5 7-seg digits
        
        uint8_t _mapSegments(uint8_t val);
        int8_t _get7SegmentsAddress(uint8_t row, uint8_t col);
};
#endif
