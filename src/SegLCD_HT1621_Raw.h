#ifndef SEGLCD_HT1621_RAW_H
#define SEGLCD_HT1621_RAW_H

#include <SegDriver_HT1621.h>


class SegLCD_HT1621_Raw : public SegDriver_HT1621 {
    public:
        SegLCD_HT1621_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init(ModeDrive drive, ModeBias bias);
        void write(uint8_t data, uint8_t address = 0);
        void write(uint8_t *data, size_t length, uint8_t address = 0);
        void sendCommand(uint8_t command);
};
#endif
