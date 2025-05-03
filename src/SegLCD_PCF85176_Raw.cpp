#include <Wire.h>
#include <SegLCD_PCF85176_Raw.h>


SegLCD_PCF85176_Raw::SegLCD_PCF85176_Raw(TwoWire& i2c, uint8_t address, uint8_t subaddress) : SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_Raw::begin(ModeDrive drive, ModeBias bias) {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED, drive, bias);
}

// For generic class allow access to low level functions
void SegLCD_PCF85176_Raw::write(uint8_t data, uint8_t address) {
    _write(data, address);
}

void SegLCD_PCF85176_Raw::write(uint8_t *data, size_t length, uint8_t address) {
    _write(data, length, address);
}
