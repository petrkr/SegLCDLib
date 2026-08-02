#if !defined(SEGLCD_DISABLE_ALL_LCDS) || defined(SEGLCD_ENABLE_PCF85176_RAW)
#include <SegLCD_PCF85176_Raw.h>


SegLCD_PCF85176_Raw::SegLCD_PCF85176_Raw(SegTransportI2C& transport, uint8_t address, uint8_t subaddress) : SegDriver_PCF85176(transport, address, subaddress) {
    _allocateBuffer((MAX_ADDRESS / 2) + 1);
}

void SegLCD_PCF85176_Raw::init(ModeDrive drive, ModeBias bias) {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED, drive, bias);
}

// For generic class allow access to low level functions
void SegLCD_PCF85176_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_PCF85176_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}

#endif
