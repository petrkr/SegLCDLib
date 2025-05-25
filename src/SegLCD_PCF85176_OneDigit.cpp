#include <Wire.h>
#include <SegLCD_PCF85176_OneDigit.h>

SegLCD_PCF85176_OneDigit::SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

//TODO: Support for chaining multiple displays
// For now it supports only one controller with 5 digits
#define DIGIT_COUNT 5

void SegLCD_PCF85176_OneDigit::begin() {
    begin(false);
}

void SegLCD_PCF85176_OneDigit::begin(bool v1fix) {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
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

void SegLCD_PCF85176_OneDigit::writeFloat(float input, uint8_t decimals, LCDSections section) {
    clear();

    bool isNegative = input < 0.0f;
    float scale = powf(10, decimals);
    long scaled = lroundf(fabsf(input) * scale);

    int totalDigits = _countDigits(scaled);
    int digitPos = 1;

    for (int i = 0; i < totalDigits; ++i) {
        int digit = scaled % 10;
        writeChar(digitPos, digit+'0');

        if (i == decimals && decimals > 0) {
            setDecimal(digitPos, true);
        }

        scaled /= 10;
        digitPos++;
    }

    if (isNegative) {
        writeChar(digitPos, '-');
        digitPos++;
    }
}

// Write string from left to right, support '.' as decimal point (attached to previous digit)
void SegLCD_PCF85176_OneDigit::writeString(const char* str, LCDSections section) {
    clear();
    int len = strlen(str);
    int digitIndex = 0;

    for (int i = 0; i < len && digitIndex < DIGIT_COUNT; ++i) {
        if (str[i] == '.') {
            continue;
        }

        int digit = DIGIT_COUNT - digitIndex;
        writeChar(digit, str[i], section);

        if (i + 1 < len && str[i + 1] == '.') {
            setDecimal(digit, true, section);
        }

        digitIndex++;
    }
}
