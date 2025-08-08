#include <Arduino.h>
#include <SegDriver_HT1622.h>


SegDriver_HT1622::SegDriver_HT1622(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read)
    : SegDriver_3Wire(chipselect, data, write, read) {
    _maxAddress = 62;
}

void SegDriver_HT1622::clear() {
    uint8_t tmp[(MAX_ADDRESS + 1) / 2] = { 0 };

    _writeRam(tmp, sizeof(tmp), 0);
    SegLCDLib::clear();
}


void SegDriver_HT1622::_writeRam(uint8_t data, uint8_t address) {
    _writeRam(&data, 1, address);
}

void SegDriver_HT1622::_writeRam(uint8_t *data, size_t length, uint8_t address) {
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

void SegDriver_HT1622::_sendBits(uint16_t data, uint8_t bitCount) {
    for (int8_t i = bitCount - 1; i >= 0; i--) {
        digitalWrite(_data, (data >> i) & 1);
        delayMicroseconds(1);     // Data setup time (120ns required, 1μs safe)
        digitalWrite(_wr, LOW);
        delayMicroseconds(4);     // Write pulse LOW: 3.34μs @ 3V (4μs safe)
        digitalWrite(_wr, HIGH);
        delayMicroseconds(4);     // Write pulse HIGH: 3.34μs @ 3V (4μs safe)
    }
}
