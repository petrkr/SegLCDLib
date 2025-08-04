#ifndef SEGLCD_VT0192_RAW_H
#define SEGLCD_VT0192_RAW_H

#include <SegDriver_VK0192.h>


class SegLCD_VK0192_Raw : public SegDriver_VK0192 {
    public:
        SegLCD_VK0192_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
