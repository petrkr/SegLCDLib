#ifndef SEGLCD_HT1621_4SEGDEGREE_H
#define SEGLCD_HT1621_4SEGDEGREE_H

#include <SegDriver_HT1621.h>


class SegLCD_HT1621_4SegDegree : public SegDriver_HT1621 {
    #define ADDR_SYMBOLS 6

    public:
        SegLCD_HT1621_4SegDegree(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void begin() override;
        void setDegree(bool state, LCDSections section = LCDSections::SECTION_DEFAULT);
        void setMiddleDot(bool state, LCDSections section = LCDSections::SECTION_DEFAULT);
        void setClockColon(bool status, LCDSections section = LCDSections::SECTION_CLOCK);
        void setDecimal(uint8_t digit, bool state, LCDSections section = LCDSections::SECTION_DEFAULT);
        void writeChar(uint8_t digit, char c, LCDSections section = LCDSections::SECTION_DEFAULT) override;

    private:
        uint8_t _buffer[4] = {0};

        uint8_t _mapSegments(uint8_t val);
        void _writeSymbols(uint8_t bitnum, bool state);
};
#endif
