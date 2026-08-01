#include "SegDriver_3Wire.h"

SegDriver_3Wire::SegDriver_3Wire(SegTransport3Wire& transport, uint8_t chipselect)
    : _transport(transport), _cs(chipselect) {}

void SegDriver_3Wire::init() {
    _transport.init_cs(_cs);
    _transport.set_cs(_cs, true);
    SegLCDLib::init();
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
    _transport.set_cs(_cs, false);

    // send CMD prefix 100 (command mode)
    _transport.write(OP_CMD, 3);

    // Send 8bits command
    _transport.write(command, 8);

    // Suffix, in command mode, we always write 0
    _transport.write(0, 1);

    _transport.set_cs(_cs, true);
}

void SegDriver_3Wire::flush(uint8_t startAddr, uint8_t length) {
    if (!_ramBuffer || startAddr >= _ramBufferSize) return;

    bool prevAuto = _autoFlush;
    _autoFlush = true;

    // Clamp length to buffer bounds
    if (startAddr + length > _ramBufferSize) {
        length = _ramBufferSize - startAddr;
    }

    // 3-wire protocol writes individual addresses (no bulk write support)
    // Loop through each byte in the range
    for (uint8_t i = 0; i < length; i++) {
        uint8_t addr = (startAddr + i) * 2;  // Convert byte index to nibble address
        _writeRam(_ramBuffer[startAddr + i], addr);
    }

    _autoFlush = prevAuto;
}

void SegDriver_3Wire::_writeRam(uint8_t *data, size_t length, uint8_t address) {
    if (!_autoFlush) {
        return;
    }
    _transport.set_cs(_cs, false);

    _transport.write(OP_WRITE, 3);

    // Send 6 bit address
    _transport.write(address, 6);

    // Send data
    for (size_t i = 0; i < length; i++) {
        _transport.write(data[i], 8);
    }

    _transport.set_cs(_cs, true);
}
