#include <Arduino.h>
#include <SegDriver_VK0192.h>


SegDriver_VK0192::SegDriver_VK0192(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read)
    : SegDriver_3Wire(chipselect, data, write, read) {
    _maxAddress = 47;
}

void SegDriver_VK0192::clear() {
    uint8_t tmp[(MAX_ADDRESS + 1) / 2] = { 0 };

    _writeRam(tmp, sizeof(tmp), 0);
    SegLCDLib::clear();
}

void SegDriver_VK0192::flush(uint8_t startAddr, uint8_t length) {
    if (!_ramBuffer || startAddr >= _ramBufferSize) return;

    bool prevAuto = _autoFlush;
    _autoFlush = true;

    // Clamp length to buffer bounds
    if (startAddr + length > _ramBufferSize) {
        length = _ramBufferSize - startAddr;
    }

    // VK0192 writes individual addresses (no bulk write support)
    // Loop through each byte in the range
    for (uint8_t i = 0; i < length; i++) {
        uint8_t addr = (startAddr + i) * 2;  // Convert byte index to nibble address
        _writeRam(_ramBuffer[startAddr + i], addr);
    }

    _autoFlush = prevAuto;
}
