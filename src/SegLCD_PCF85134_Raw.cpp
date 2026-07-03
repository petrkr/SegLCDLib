#include <SegLCD_PCF85134_Raw.h>


SegLCD_PCF85134_Raw::SegLCD_PCF85134_Raw(SegTransportI2C& transport, uint8_t address, uint8_t subaddress) : SegDriver_PCF85134(transport, address, subaddress) {
    _allocateBuffer((MAX_ADDRESS / 2) + 1);
}

void SegLCD_PCF85134_Raw::init(ModeDrive drive, ModeBias bias) {
    SegDriver_PCF85134::init();
    _setMode(MODE_STATUS_ENABLED, drive, bias);
}

size_t SegLCD_PCF85134_Raw::write(uint8_t ch) {
    (void)ch;
    return 1;
}

// For generic class allow access to low level functions
void SegLCD_PCF85134_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_PCF85134_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
