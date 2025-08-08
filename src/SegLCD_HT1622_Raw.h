#ifndef SEGLCD_HT1622_RAW_H
#define SEGLCD_HT1622_RAW_H

#include <SegDriver_HT1622.h>


class SegLCD_HT1622_Raw : public SegDriver_HT1622 {
    public:
        SegLCD_HT1622_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
