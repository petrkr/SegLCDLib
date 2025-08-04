#ifndef SEGLCD_HT1621_4SEGDEGREE_H
#define SEGLCD_HT1621_4SEGDEGREE_H

#include <SegDriver_HT1621.h>


class SegLCD_HT1621_4SegDegree : public SegDriver_HT1621 {
    #define ADDR_SYMBOLS 6

    public:
        SegLCD_HT1621_4SegDegree(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void clear();
        void setDegree(bool state);
        void setMiddleDot(bool state);
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;

    private:
        uint8_t _buffer[4] = {0};

        static constexpr uint8_t DIGITS = 4;

        uint8_t _mapSegments(uint8_t val);
        void _writeSymbols(uint8_t bitnum, bool state);
};
#endif
