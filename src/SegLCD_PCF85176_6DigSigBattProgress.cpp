#include <Wire.h>
#include <SegLCD_PCF85176_6DigSigBattProgress.h>


SegLCD_PCF85176_6DigitSignalBatteryProgress::SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::begin() {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setBatteryLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0x0f);

    if (level > 0)
        _buffer_sigbatt |= 8;
    if (level > 1)
        _buffer_sigbatt |= 1;
    if (level > 2)
        _buffer_sigbatt |= 2;
    if (level > 3)
        _buffer_sigbatt |= 4;

    _write(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setSignalLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0xf0);

    if (level > 0)
        _buffer_sigbatt |= 0x80;
    if (level > 1)
        _buffer_sigbatt |= 0x40;
    if (level > 2)
        _buffer_sigbatt |= 0x20;
    if (level > 3)
        _buffer_sigbatt |= 0x10;

    _write(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setProgress(uint8_t value) {
    uint8_t buffer[2] = { 0, 0 };

    if (value > 0) {
        buffer[0] |= 0x80;
    }
    if (value >= 10) {
        buffer[0] |= 0x40;
    }
    if (value >= 20) {
        buffer[0] |= 0x20;
    }
    if (value >= 30) {
        buffer[0] |= 0x10;
    }
    if (value >= 40) {
        buffer[0] |= 0x08;
    }
    if (value >= 50) {
        buffer[0] |= 0x04;
    }
    if (value >= 60) {
        buffer[0] |= 0x02;
    }
    if (value >= 70) {
        buffer[0] |= 0x01;
    }

    if (value >= 80) {
        buffer[1] |= 0x80;
    }
    if (value >= 90) {
        buffer[1] |= 0x40;
    }
    if (value >= 100) {
        buffer[1] |= 0x20;
    }
    if (value >= 110) {
        buffer[1] |= 0x10;
    }
    if (value >= 120) {
        buffer[1] |= 0x08;
    }
    if (value >= 130) {
        buffer[1] |= 0x04;
    }
    if (value >= 140) {
        buffer[1] |= 0x02;
    }
    if (value == 150) {
        buffer[1] |= 0x01;
    }

    _write(buffer, sizeof(buffer), ADDR_PROGRESS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setWheel(uint16_t value) {
    uint8_t buffer[2] = { 0, 0 };
    
    buffer[0] = value & 0xFF;
    buffer[1] = (((value >> 8) & 0x0F) << 4);

    _write(buffer, sizeof(buffer), ADDR_WHEEL);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setLabels(LabelFlags labels) {
    _buffer_labels |= labels;
    
    _write(_buffer_labels, ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::clearLabels(LabelFlags labels) {
    _buffer_labels &= ~labels;
    
    _write(_buffer_labels, ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::writeChar(uint8_t digit, char c, LCDSections section) {
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
            _write(_mapSegments(_get_char_value(c)), ADDR_BIG_SEGS + ((6 - digit) * 2));
            break;
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
             _write(_mapSegments(_get_char_value(c)), ADDR_SMALL_SEGS + ((digit - 1) * 2));
            break;
    }
}

//Mapping ABCD EFGH
uint8_t SegLCD_PCF85176_6DigitSignalBatteryProgress::_get_char_value(char ch) {
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

        case '*': return 0b11000110;
        case ',': return 0b00001000;
        case '.': return 0b00000001;
        case '/': return 0b01001010;
        case '~': return 0b10000000;
        case '-': return 0b00000010;
        case '_': return 0b00010000;
        case ':': return 0b00010010;
        case '!': return 0b01100001;
        case '|': return 0b00001100;
        case '"': return 0b01000100;
        case '(':
        case '[': return 0b10011100;
        case ')':
        case ']': return 0b11110000;

        default: return 0x00;
    }
}

// ABCD_EFGH to ABCH FGED
uint8_t SegLCD_PCF85176_6DigitSignalBatteryProgress::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11100000);           // ABC: bits 7-5 → 7-5
    out |= (val & 0b00010000) >> 4;      // D: bit 4 → 0
    out |= (val & 0b00001000) >> 2;      // E: bit 3 → 1
    out |= (val & 0b00000100) << 1;      // F: bit 2 → 3
    out |= (val & 0b00000010) << 1;      // G: bit 1 → 2
    out |= (val & 0b00000001) << 4;      // H: bit 0 → 4
    return out;
}