#include "SegDriver_3Wire.h"

SegDriver_3Wire::SegDriver_3Wire(SegTransport3Wire& transport, uint8_t chipselect)
    : _transport(transport), _cs(chipselect) {}

void SegDriver_3Wire::init() {
    pinMode(_cs, OUTPUT);
    _transport.set_cs(_cs, HIGH);
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
    _transport.set_cs(_cs, LOW);

    // send CMD prefix 100 (command mode)
    _transport.write(OP_CMD, 3);

    // Send 8bits command
    _transport.write(command, 8);

    // Suffix, in command mode, we always write 0
    _transport.write(0, 1);

    _transport.set_cs(_cs, HIGH);
}

void SegDriver_3Wire::_writeRam(uint8_t *data, size_t length, uint8_t address) {
    if (!_autoFlush) {
        return;
    }
    _transport.set_cs(_cs, LOW);

    _transport.write(OP_WRITE, 3);

    // Send 6 bit address
    _transport.write(address, 6);

    // Send data
    for (size_t i = 0; i < length; i++) {
        _transport.write(data[i], 8);
    }

    _transport.set_cs(_cs, HIGH);
}
