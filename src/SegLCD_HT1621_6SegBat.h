#ifndef SEGLCD_HT1621_6SEGBAT_H
#define SEGLCD_HT1621_6SEGBAT_H

#include <SegDriver_HT1621.h>


class SegLCD_HT1621_6SegBat : public SegDriver_HT1621 {
    #define ADDR_BATT 6
    public:
        SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        void setDecimal(uint8_t digit, bool state, LCDSections section = LCDSections::SECTION_DEFAULT);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;

    private:
        uint8_t _buffer_default[6] = {0};

        uint8_t _mapSegments(uint8_t val);
};
#endif
