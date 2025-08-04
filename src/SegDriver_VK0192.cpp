#include <Arduino.h>
#include <SegDriver_VK0192.h>


SegDriver_VK0192::SegDriver_VK0192(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) {
    _cs = chipselect;
    _data = data;
    _wr = write;
    _rd = read;
    _irq = -1; // Not used, but reserved for future use
}

void SegDriver_VK0192::init() {
    pinMode(_cs, OUTPUT);
    pinMode(_data, OUTPUT);
    pinMode(_wr, OUTPUT);
    if (_rd > -1) {
        pinMode(_rd, OUTPUT);
    }

    // Set the chip select to high
    digitalWrite(_cs, HIGH);
}

void SegDriver_VK0192::clear() {
    uint8_t tmp[(MAX_ADDRESS + 1) / 2] = { 0 };

    _writeRam(tmp, sizeof(tmp), 0);
    SegLCDLib::clear();
}

void SegDriver_VK0192::on() {
    // Enable the display
    command(CMD_LCD_ON);
}

void SegDriver_VK0192::off() {
    // Disable the display
    command(CMD_LCD_OFF);
}

void SegDriver_VK0192::command(uint8_t command) {
    digitalWrite(_cs, LOW);

    // send CMD prefix 100 (command mode)
    _sendBits(OP_CMD, 3);

    // Send 8bits command
    _sendBits(command);

    // Suffix, in command mode, we always write 0
    _sendBits(0, 1);

    digitalWrite(_cs, HIGH);
}

void SegDriver_VK0192::_writeRam(uint8_t data, uint8_t address) {
    _writeRam(&data, 1, address);
}

void SegDriver_VK0192::_writeRam(uint8_t *data, size_t length, uint8_t address) {
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

// Private method to send bits
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
