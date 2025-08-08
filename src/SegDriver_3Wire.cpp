#include "SegDriver_3Wire.h"

SegDriver_3Wire::SegDriver_3Wire(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) {
    _cs = chipselect;
    _data = data;
    _wr = write;
    _rd = read;
    _irq = -1; // Not used, but reserved for future use
    _maxAddress = 0; // Will be set by derived classes
}

void SegDriver_3Wire::init() {
    pinMode(_cs, OUTPUT);
    pinMode(_data, OUTPUT);
    pinMode(_wr, OUTPUT);
    if (_rd > -1) {
        pinMode(_rd, OUTPUT);
    }

    // Set the chip select to high
    digitalWrite(_cs, HIGH);
}

void SegDriver_3Wire::on() {
    // Enable the display
    command(CMD_LCD_ON);
}

void SegDriver_3Wire::off() {
    // Disable the display
    command(CMD_LCD_OFF);
}

void SegDriver_3Wire::command(uint8_t command) {
    digitalWrite(_cs, LOW);

    // send CMD prefix 100 (command mode)
    _sendBits(OP_CMD, 3);

    // Send 8bits command
    _sendBits(command);

    // Suffix, in command mode, we always write 0
    _sendBits(0, 1);

    digitalWrite(_cs, HIGH);
}

void SegDriver_3Wire::_writeRam(uint8_t data, uint8_t address) {
    _writeRam(&data, 1, address);
}

void SegDriver_3Wire::_writeRam(uint8_t *data, size_t length, uint8_t address) {
    digitalWrite(_cs, LOW);

    _sendBits(OP_WRITE, 3);

    // Send 6 bit address
    _sendBits(address, 6);

    // Send data
    for (size_t i = 0; i < length; i++) {
        _sendBits(data[i], 8);
    }

    digitalWrite(_cs, HIGH);
}

void SegDriver_3Wire::_sendBits(uint16_t data, uint8_t bitCount) {
    for (int8_t i = bitCount - 1; i >= 0; i--) {
        digitalWrite(_data, (data >> i) & 1);
        digitalWrite(_wr, LOW);
        delayMicroseconds(1);
        digitalWrite(_wr, HIGH);
        delayMicroseconds(1);
    }
}