#include <SegLCD_HT1622_Raw.h>


SegLCD_HT1622_Raw::SegLCD_HT1622_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1622(chipselect, data, write, read) {}

// For generic class allow access to low level functions
void SegLCD_HT1622_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_HT1622_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
