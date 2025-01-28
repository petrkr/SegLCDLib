#ifndef SEGLCD_PCF85176_ONEDIGIT_H
#define SEGLCD_PCF85176_ONEDIGIT_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_OneDigit : public SegDriver_PCF85176 {

    #define MAX_ADDRESS 39

    public:
        SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0);
        void begin() override;
        void begin(bool v1fix);
        void writeChar(uint8_t digit, char c, LCDSections section = LCDSections::SECTION_DEFAULT) override;

    private:
        bool _v1fix;
        uint8_t _get_char_value(char ch);
};
#endif
