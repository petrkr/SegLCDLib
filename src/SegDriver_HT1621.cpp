#include <Arduino.h>
#include <SegDriver_HT1621.h>


SegDriver_HT1621::SegDriver_HT1621(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) {
    _cs = chipselect;
    _data = data;
    _wr = write;
    _rd = read;
    _irq = -1; // Not used, but reserved for future use
}

void SegDriver_HT1621::init() {
    pinMode(_cs, OUTPUT);
    pinMode(_data, OUTPUT);
    pinMode(_wr, OUTPUT);
    if (_rd > -1) {
        pinMode(_rd, OUTPUT);
    }

    // Set the chip select to high
    digitalWrite(_cs, HIGH);
}

void SegDriver_HT1621::clear() {
    // TODO: Make this more generic or dynamic
    uint8_t tmp[(MAX_ADDRESS / 2)+1] = { 0 };

    switch (_drive) {
        case MODE_DRIVE_STATIC:
            return;
        //TODO: Get riht size values here
        case MODE_DRIVE_12:
        case MODE_DRIVE_13:
            break;
        case MODE_DRIVE_14:
            _writeRam(tmp, sizeof(tmp), 0);
            return;
        default:
            break;
    }
}

void SegDriver_HT1621::on() {
    // Enable the display
    command(CMD_LCD_ON);
}

void SegDriver_HT1621::off() {
    // Disable the display
    command(CMD_LCD_OFF);
}

void SegDriver_HT1621::command(uint8_t command) {
    digitalWrite(_cs, LOW);

    // send CMD prefix 100 (command mode)
    _sendBits(OP_CMD, 3);

    // Send 8bits command
    _sendBits(command);

    // Suffix, in command mode, we always write 0
    _sendBits(0, 1);

    digitalWrite(_cs, HIGH);
}

void SegDriver_HT1621::_writeRam(uint8_t data, uint8_t address) {
    _writeRam(&data, 1, address);
}

void SegDriver_HT1621::_writeRam(uint8_t *data, size_t length, uint8_t address) {
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

void SegDriver_HT1621::_setMode(ModeDrive drive, ModeBias bias) {
    _drive = drive; // Store the current drive mode
    uint8_t data;

    data = 0; // Clear data

    // Set bias mode
    switch (bias) {
        case MODE_BIAS_12:
            data |= CMD_BIAS_12; // Set bias mode 1/2
            break;
        case MODE_BIAS_13:
            data |= CMD_BIAS_13; // Set bias mode 1/3
            break;
        default:
            return; // Invalid bias mode, do nothing
    }

    switch (drive) {
        case MODE_DRIVE_STATIC:
            // we do not support static drive mode
            break;
        case MODE_DRIVE_12:
            data |= DRIVE_MODE_12_MASK; // 1/2 multiplexing drive mode
            break;
        case MODE_DRIVE_13:
            data |= DRIVE_MODE_13_MASK; // 1/3 multiplexing drive mode
            break;
        case MODE_DRIVE_14:
            data |= DRIVE_MODE_14_MASK; // 1/4 multiplexing drive mode
            break;
        default:
            return; // Invalid drive mode, do nothing
            break;
    }

    command(data);
}

// Private method to send bits
void SegDriver_HT1621::_sendBits(uint16_t data, uint8_t bitCount) {
  for (int8_t i = bitCount - 1; i >= 0; i--) {
    digitalWrite(_data, (data >> i) & 1);
    digitalWrite(_wr, LOW);
    delayMicroseconds(1);
    digitalWrite(_wr, HIGH);
    delayMicroseconds(1);
  }
}
