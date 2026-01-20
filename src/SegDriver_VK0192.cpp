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


void SegDriver_VK0192::_writeRam(uint8_t data, uint8_t address) {
    _writeRam(&data, 1, address);
}

void SegDriver_VK0192::_writeRam(uint8_t *data, size_t length, uint8_t address) {
    if (!_autoFlush) {
        return;
    }
    digitalWrite(_cs, LOW);
    delayMicroseconds(1);     // CS setup time (100ns required, 1μs safe)

    _sendBits(OP_WRITE, 3);

    // Send 6 bit address
    _sendBits(address, 6);

    // Send data
    for (size_t i = 0; i < length; i++) {
        _sendBits(data[i], 8);
    }

    delayMicroseconds(1);     // CS hold time (100ns required, 1μs safe)
    digitalWrite(_cs, HIGH);
}

void SegDriver_VK0192::_sendBits(uint16_t data, uint8_t bitCount) {
    for (int8_t i = bitCount - 1; i >= 0; i--) {
        digitalWrite(_data, (data >> i) & 1);
        delayMicroseconds(1);     // Data setup time (120ns required, 1μs safe)
        digitalWrite(_wr, LOW);
        delayMicroseconds(4);     // Write pulse LOW: 3.34μs @ 3V (4μs safe)
        digitalWrite(_wr, HIGH);
        delayMicroseconds(4);     // Write pulse HIGH: 3.34μs @ 3V (4μs safe)
    }
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
