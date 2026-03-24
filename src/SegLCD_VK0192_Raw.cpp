#include <SegLCD_VK0192_Raw.h>


SegLCD_VK0192_Raw::SegLCD_VK0192_Raw(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_VK0192(transport, chipselect) {
    _allocateBuffer((MAX_ADDRESS / 2) + 1);
}

// For generic class allow access to low level functions
void SegLCD_VK0192_Raw::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_VK0192_Raw::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
