#include <SegLCD_PCx85_Raw.h>


SegLCD_PCx85_Raw::SegLCD_PCx85_Raw(TwoWire& i2c, uint8_t address, uint8_t subaddress) : SegDriver_PCx85(i2c, address, subaddress) {}

void SegLCD_PCx85_Raw::init(ModeDrive drive, ModeBias bias) {
    SegDriver_PCx85::init();
    _setMode(MODE_STATUS_ENABLED, drive, bias);
}

// For generic class allow access to low level functions
void SegLCD_PCx85_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_PCx85_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
