#include <SegLCD_VK0192_Raw.h>


SegLCD_VK0192_Raw::SegLCD_VK0192_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_VK0192(chipselect, data, write, read) {}

// For generic class allow access to low level functions
void SegLCD_VK0192_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_VK0192_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
