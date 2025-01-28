#include <Wire.h>
#include <SegLCD_PCF85176_OneDigit.h>


SegLCD_PCF85176_OneDigit::SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_OneDigit::begin() {
    begin(false);
}

void SegLCD_PCF85176_OneDigit::begin(bool v1fix) {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED, MODE_BIAS_13, MODE_DRIVE_STATIC);
    _v1fix = v1fix;
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

    _write(b, (digit-1)*8);
}

uint8_t SegLCD_PCF85176_OneDigit::_get_char_value(char ch) {
    // Segments mapping order ABCD EFGH
    switch (ch) {
        case ' ': return 0x00;
        case '0': return 0xFC;
        case '1': return 0x60;
        case '2': return 0xDA;
        case '3': return 0xF2;
        case '4': return 0x66;
        case '5': return 0xB6;
        case '6': return 0xBE;
        case '7': return 0xE0;
        case '8': return 0xFE;
        case '9': return 0xF6;

        default:  return 0x00; // Return 0x00 for unsupported characters
    }
}
