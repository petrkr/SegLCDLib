#include <Wire.h>
#include <SegLCD_PCF85176_T1T2Lcd.h>


SegLCD_PCF85176_T1T2Lcd::SegLCD_PCF85176_T1T2Lcd(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_T1T2Lcd::begin() {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_T1T2Lcd::clear() {
    memset(_bufferT1, 0x00, sizeof(_bufferT1));
    memset(_bufferT2, 0x00, sizeof(_bufferT2));
    SegDriver_PCx85::clear();
}

void SegLCD_PCF85176_T1T2Lcd::setBatteryLevel(uint8_t level) {
    if (level > 4)
        level = 4;

        _buffer_batt &= ~(0xf0);

    if (level > 0)
        _buffer_batt |= 0x10;
    if (level > 1)
        _buffer_batt |= 0x80;
    if (level > 2)
        _buffer_batt |= 0x40;
    if (level > 3)
        _buffer_batt |= 0x20;

    _write(_buffer_batt, ADDR_BATT);
}

void SegLCD_PCF85176_T1T2Lcd::setSignalLevel(uint8_t level) {
    if (level > 5)
        level = 5;

        _buffer_sigclk &= ~(0xf8);

    if (level > 0)
        _buffer_sigclk |= 0x80;
    if (level > 1)
        _buffer_sigclk |= 0x40;
    if (level > 2)
        _buffer_sigclk |= 0x20;
    if (level > 3)
        _buffer_sigclk |= 0x10;
    if (level > 4)
        _buffer_sigclk |= 0x08;

    _write(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setClockSymbol(bool status) {
    if (status)
        _buffer_sigclk |= 0x04;
    else
        _buffer_sigclk &= ~0x04;

    _write(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setClockColon(bool status) {
    if (status)
        _buffer_sigclk |= 0x02;
    else
        _buffer_sigclk &= ~0x02;

    _write(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setLabels(uint8_t labels) {
    _buffer_labels |= labels;
    _write(_buffer_labels, ADDR_LABELS);
}

void SegLCD_PCF85176_T1T2Lcd::clearLabels(uint8_t labels) {
    _buffer_labels &= ~labels;
    _write(_buffer_labels, ADDR_LABELS);
}

void SegLCD_PCF85176_T1T2Lcd::setT1T2Labels(uint8_t t1t2) {
    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _buffer_clock[0] |= 0x01;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _buffer_clock[1] |= 0x01;

    _write(_buffer_clock, 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::clearT1T2Labels(uint8_t t1t2) {
    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _buffer_clock[0] &= ~0x01;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _buffer_clock[1] &= ~0x01;

    _write(_buffer_clock, 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::setDecimal(uint8_t digit, bool state, LCDSections section) {
    if (digit < 1 || digit > 4) {
        return; // Invalid digit
    }

    uint8_t address = 0;
    uint8_t* _buffer = nullptr;
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
        case LCDSections::SECTION_T1:
            address = ADDR_T1_SEGS + ((digit - 1) * 2);
            _buffer = _bufferT1;
            break;
        case LCDSections::SECTION_T2:
            address = ADDR_T2_SEGS + ((digit - 1) * 2);
            _buffer= _bufferT2;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[(digit-1)] |= 0x01; // Set the decimal point bit
    } else {
        _buffer[(digit-1)] &= ~0x01; // Clear the decimal point bit
    }

    _write(_buffer[(digit-1)], address);
}

void SegLCD_PCF85176_T1T2Lcd::writeFloat(float input, uint8_t decimals, LCDSections section) {
    bool isNegative = input < 0.0f;
    float scale = powf(10, decimals);
    long scaled = lroundf(fabsf(input) * scale);

    int totalDigits = _countDigits(scaled);
    int digitPos = 1;

    for (int i = 0; i < totalDigits; ++i) {
        int digit = scaled % 10;
        writeChar(4-digitPos, digit+'0', section);

        if (i == decimals && decimals > 0) {
            setDecimal(4-digitPos, true, section);
        }

        scaled /= 10;
        digitPos++;
    }

    if (isNegative) {
        writeChar(4-digitPos, '-', section);
        digitPos++;
    }
}

    void SegLCD_PCF85176_T1T2Lcd::writeChar(uint8_t digit, char c, LCDSections section) {
    uint8_t ch = _mapSegments(_get_char_value(c));

    // TODO: Do some dot bit mask and make it more generic
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
        case LCDSections::SECTION_T1:
            _bufferT1[digit - 1] &= ~0b11111110;
            _bufferT1[digit - 1] |= ch & 0b11111110;
            _write(_bufferT1[digit - 1], ADDR_T1_SEGS + ((digit - 1) * 2));
            break;
        case LCDSections::SECTION_T2:
            _bufferT2[digit - 1] &= ~0b11111110;
            _bufferT2[digit - 1] |= ch & 0b11111110;
            _write(_bufferT2[digit - 1], ADDR_T2_SEGS + ((digit - 1) * 2));
            break;    
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
            _buffer_clock[digit - 1] &= ~0b11111110;
            _buffer_clock[digit - 1] |= ch & 0b11111110;
             _write(_buffer_clock[digit - 1], ADDR_CLOCK_T1T2_LABELS_SEGS + ((digit - 1) * 2));
            break;
        case LCDSections::SECTION_DAY:
             _write(ch, ADDR_DAY_SEG + ((digit - 1) * 2));
            break;
    }
}

//Mapping ABCD EFGH
uint8_t SegLCD_PCF85176_T1T2Lcd::_get_char_value(char ch) {
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

// ABCD_EFGH to FGED ABCH..
uint8_t SegLCD_PCF85176_T1T2Lcd::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11100000) >> 4;   // ABC
    out |= (val & 0b00010000);        // D
    out |= (val & 0b00001000) << 2;   // E
    out |= (val & 0b00000110) << 5;   // FG
    out |= (val & 0b00000001);        // H
    return out;
}

// TODO: Move it to generic class
int SegLCD_PCF85176_T1T2Lcd::_countDigits(long num) {
    int count = 0;
    do {
        count++;
        num /= 10;
    } while (num > 0);
    return count;
}

// For generic class allow access to low level functions
void SegLCD_PCF85176_T1T2Lcd::write(uint8_t data, uint8_t address) {
    _write(data, address);
}