#include <SegLCD_HT1621_LCM0844.h>


SegLCD_HT1621_LCM0844::SegLCD_HT1621_LCM0844(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_HT1621_LCM0844::init() {
    SegDriver_HT1621::init();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}


void SegLCD_HT1621_LCM0844::setBatteryLevel(uint8_t level) {

    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    uint8_t data = 0;

    if (level >= 1) data |= 0x40; // outline
    if (level >= 2) data |= 0x08; // line 1
    if (level >= 3) data |= 0x04; // line 2
    if (level >= 4) data |= 0x02; // line 3
    if (level >= 5) data |= 0x01; // line 4

    // Use masked write to preserve digit segments (only modify battery bit 0x80)
    _writeRamMasked(data, BATTERY_LEVEL_ADR, BATTERY_MASK);
}

void SegLCD_HT1621_LCM0844::setLoadLevel(uint8_t level) {

    //if (level > MAX_LOAD_LEVEL)
    //    level = MAX_LOAD_LEVEL;

    uint8_t data = 0;

    if (level >= 1) data |= 0x80; // outline
    if (level >= 2) data |= 0x08; // line 1
    if (level >= 3) data |= 0x04; // line 2
    if (level >= 4) data |= 0x02; // line 3
    if (level >= 5) data |= 0x01; // line 4

    // Use masked write to preserve digit segments (only modify battery bit 0x80)
    _writeRamMasked(data, LOAD_LEVEL_ADR, LOAD_MASK);
}

void SegLCD_HT1621_LCM0844::_updateLabels(uint32_t labels, bool set) {
    for (size_t i = 0; i < sizeof(LABEL_MAP) / sizeof(LABEL_MAP[0]); i++) {
        const AddressMapping& entry = LABEL_MAP[i];
        uint8_t bits = 0;
        for (uint8_t j = 0; entry.bits[j].flag; j++) {
            if (labels & entry.bits[j].flag) {
                bits |= entry.bits[j].bit;
            }
        }
        if (bits) {
            _writeRamMasked(set ? bits : 0x00, entry.address, bits);
        }
    }
}

void SegLCD_HT1621_LCM0844::_updateSymbols(uint32_t symbols, bool set) {
    for (size_t i = 0; i < sizeof(SYMBOL_MAP) / sizeof(SYMBOL_MAP[0]); i++) {
        const AddressMapping& entry = SYMBOL_MAP[i];
        uint8_t bits = 0;
        for (uint8_t j = 0; entry.bits[j].flag; j++) {
            if (symbols & entry.bits[j].flag) {
                bits |= entry.bits[j].bit;
            }
        }
        if (bits) {
            _writeRamMasked(set ? bits : 0x00, entry.address, bits);
        }
    }
}

void SegLCD_HT1621_LCM0844::setLabels(uint32_t labels) {
    _updateLabels(labels, true);
}

void SegLCD_HT1621_LCM0844::clearLabels(uint32_t labels) {
    _updateLabels(labels, false);
}

void SegLCD_HT1621_LCM0844::setSymbols(uint32_t symbols) {
    _updateSymbols(symbols, true);
}

void SegLCD_HT1621_LCM0844::clearSymbols(uint32_t symbols) {
    _updateSymbols(symbols, false);
}

void SegLCD_HT1621_LCM0844::_setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row != 0) {
        return;
    }

    int8_t hw_addr = -1;
    for (uint8_t i = 0; i < (sizeof(DECIMAL_DIGITS) / sizeof(DECIMAL_DIGITS[0])); i++) {
        if (DECIMAL_DIGITS[i] != col) {
            continue;
        }

        uint8_t digit_offset = 2;
        if (col > 4) {
            digit_offset = 6;
        }

        hw_addr = (col * 2) + digit_offset;
        break;
    }

    if (hw_addr < 0) {
        return;
    }

    uint8_t data = state ? DECIMAL_POINT_BIT : 0x00;
    _writeRamMasked(data, (uint8_t)hw_addr, DECIMAL_POINT_BIT);
}

size_t SegLCD_HT1621_LCM0844::write(uint8_t ch) {
    if (_cursorCol < 0 || _cursorCol >= DIGITS) {
        return 0;
    }

    if (ch == '.' && _cursorCol > 0) {
        _setDecimal(0, _cursorCol + DECIMAL_COL_OFFSET, true);
        return 1;
    }

    _setDecimal(0, _cursorCol, false);

    uint8_t segment_data = _mapSegments(_get_char_value(ch));
    uint8_t digit_offset = 2;
    if (_cursorCol > 2) {
        digit_offset = 4;
    }
    if (_cursorCol > 4) {
        digit_offset = 6;
    }

    uint8_t hw_addr = (_cursorCol * 2) + digit_offset;

    _writeRamMasked(segment_data, hw_addr, ~DECIMAL_POINT_BIT);

    _cursorCol++;
    return 1;
}

// ABCD_EFGH to FGED_ABCH
uint8_t SegLCD_HT1621_LCM0844::_mapSegments(uint8_t val) {
    uint8_t out = 0;

    out |= (val & 0b11100000) >> 4;  // ABC
    out |= (val & 0b00010000);       // D: bit4 -> bit4
    out |= (val & 0b00001000) << 2;  // E: bit3 -> bit5
    out |= (val & 0b00000110) << 5;  // FG

    return out;
}
