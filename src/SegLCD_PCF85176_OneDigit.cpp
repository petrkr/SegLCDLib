#include <Wire.h>
#include <SegLCD_PCF85176_OneDigit.h>

//TODO: Support for chaining multiple displays
// For now it supports only one controller with 5 digits

SegLCD_PCF85176_OneDigit::SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

// Segments mapping order ABCD EFGH
const uint8_t segment_num[10] = {
    0xFC,
    0x60,
    0xDA,
    0xF2,
    0x66,
    0xB6,
    0xBE,
    0xE0,
    0xFE,
    0xF6
};

void SegLCD_PCF85176_OneDigit::begin() {
    begin(false);
}

void SegLCD_PCF85176_OneDigit::begin(bool v1fix) {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED, MODE_BIAS_13, MODE_DRIVE_STATIC);
    _v1fix = v1fix;
}

void SegLCD_PCF85176_OneDigit::clear() {
    memset(_buffer, 0x00, sizeof(_buffer));
    SegDriver_PCx85::clear();
}

void SegLCD_PCF85176_OneDigit::writeChar(uint8_t digit, char c, LCDSections section) {
    uint8_t b = _get_char_value(c);

    if (_v1fix) {
        bool bit_2 = (b >> 2) & 1;
        bool bit_3 = (b >> 3) & 1;

        if (bit_2 != bit_3) {
            b ^= (1 << 2) | (1 << 3);
        }
    }

    _buffer[(digit-1)] = b;
    _write(b, (digit-1)*8);
}

void SegLCD_PCF85176_OneDigit::setDecimal(uint8_t digit, bool state, LCDSections section) {
    if (digit < 1 || digit > 5) {
        return; // Invalid digit
    }

    if (state) {
        _buffer[(digit-1)] |= 0x01; // Set the decimal point bit
    } else {
        _buffer[(digit-1)] &= ~0x01; // Clear the decimal point bit
    }

    _write(_buffer[(digit-1)], (digit-1)*8);
}

void SegLCD_PCF85176_OneDigit::writeFloat(float f, uint8_t decimal, LCDSections section) {
// TODO
}

// Write string from left to right
void SegLCD_PCF85176_OneDigit::writeString(const char* str, LCDSections section) {
    for (uint8_t i = 0; i < 5; i++) {
        if (str[i] == '\0') {
            break;
        }
        writeChar(i + 1, str[i], section);
    }
}


uint8_t SegLCD_PCF85176_OneDigit::_get_char_value(char ch) {
    switch (ch) {
        case ' ':
            return 0x00;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return segment_num[ch - '0'];

        default:
            return 0x00; // Return 0x00 for unsupported characters
    }
}
