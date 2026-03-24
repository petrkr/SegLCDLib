#include <SegLCD_HT1621_Raw.h>


SegLCD_HT1621_Raw::SegLCD_HT1621_Raw(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_HT1621(transport, chipselect) {
    _allocateBuffer((MAX_ADDRESS / 2) + 1);
}

void SegLCD_HT1621_Raw::init(ModeDrive drive, ModeBias bias) {
    SegDriver_HT1621::init();
    _setMode(drive, bias);
}

// For generic class allow access to low level functions
void SegLCD_HT1621_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_HT1621_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
