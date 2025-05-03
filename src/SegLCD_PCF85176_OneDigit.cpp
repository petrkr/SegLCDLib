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

int SegLCD_PCF85176_OneDigit::_countDigits(long num) {
    int count = 0;
    do {
        count++;
        num /= 10;
    } while (num > 0);
    return count;
}

// TODO: Move to to some generic class and impement mapping
//Mapping ABCD EFGH
uint8_t SegLCD_PCF85176_OneDigit::_get_char_value(char ch) {
    switch (ch) {
        case ' ': return 0x00;
        case '0': return 0b11111100;
        case '1': return 0b01100000;
        case '2': return 0b11011010;
        case '3': return 0b11110010;
        case '4': return 0b01100110;
        case '5': return 0b10110110;
        case '6': return 0b10111110;
        case '7': return 0b11100000;
        case '8': return 0b11111110;
        case '9': return 0b11110110;
        case 'A':
        case 'a': return 0b11101110;
        case 'B':
        case 'b': return 0b00111110;
        case 'C': return 0b10011100;
        case 'c': return 0b00011010;
        case 'D':
        case 'd': return 0b01111010;
        case 'E':
        case 'e': return 0b10011110;
        case 'F':
        case 'f': return 0b10001110;
        case 'G':
        case 'g': return 0b11110110;
        case 'H': return 0b01101110;
        case 'h': return 0b00101110;
        case 'I': return 0b01100000;
        case 'i': return 0b00100000;
        case 'J': return 0b01111000;
        case 'j': return 0b01111000;
        case 'K':
        case 'k': return 0b10101110;
        case 'L': return 0b00011100;
        case 'l': return 0b00011000;
        case 'M':
        case 'm': return 0b10101010;
        case 'N': return 0b11101100;
        case 'n': return 0b00101010;
        case 'O': return 0b11111100;
        case 'o': return 0b00111010;
        case 'P':
        case 'p': return 0b11001110;
        case 'Q':
        case 'q': return 0b11100110;
        case 'R':
        case 'r': return 0b00001010;
        case 'S':
        case 's': return 0b10110110;
        case 'T':
        case 't': return 0b00011110;
        case 'U': return 0b01111100;
        case 'u': return 0b00111000;
        case 'V':
        case 'v': return 0b01010100;
        case 'W':
        case 'w': return 0b01010110;
        case 'X': return 0b01101110;
        case 'x': return 0b00101000;
        case 'Y':
        case 'y': return 0b01110110;
        case 'Z': return 0b11011010;
        case 'z': return 0b11011000;
        default: return 0x00;
    }
}
