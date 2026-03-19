#include <SegLCD_HT1621_LCM59011.h>


SegLCD_HT1621_LCM59011::SegLCD_HT1621_LCM59011(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_HT1621_LCM59011::init() {
    SegDriver_HT1621::init();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_HT1621_LCM59011::setBatteryLevel(uint8_t level) {
    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    // Battery spans two addresses:
    // 0x0A: outline (0x01), line1 (0x02), line2 (0x04)
    // 0x0C: line3 (0x40), line4 (0x20), line5 (0x10)
    uint8_t data_0a = 0;
    uint8_t data_0c = 0;

    if (level >= 1) data_0a |= 0x01; // outline
    if (level >= 2) data_0a |= 0x02; // line 1
    if (level >= 3) data_0a |= 0x04; // line 2
    if (level >= 4) data_0c |= 0x40; // line 3
    if (level >= 5) data_0c |= 0x20; // line 4
    if (level >= 6) data_0c |= 0x10; // line 5

    _writeRamMasked(data_0a, 0x0A, 0x07);  // bits 0,1,2
    _writeRamMasked(data_0c, 0x0C, 0x70);  // bits 4,5,6
}

void SegLCD_HT1621_LCM59011::setSignalLevel(uint8_t level) {
    if (level > MAX_SIGNAL_LEVEL)
        level = MAX_SIGNAL_LEVEL;

    uint8_t data = 0;
    if (level >= 1) data |= 0x01; // wifi 1
    if (level >= 2) data |= 0x02; // wifi 2
    if (level >= 3) data |= 0x04; // wifi 3
    if (level >= 4) data |= 0x08; // wifi 4

    _writeRamMasked(data, SIGNAL_ADDRESS, SIGNAL_MASK);
}

void SegLCD_HT1621_LCM59011::_updateUnits(uint8_t units, bool set) {
    size_t count;
    const AddressMapping* map = _getUnitMap(count);
    for (size_t i = 0; i < count; i++) {
        const AddressMapping& entry = map[i];
        uint8_t bits = 0;
        for (uint8_t j = 0; entry.bits[j].flag; j++) {
            if (units & entry.bits[j].flag) {
                bits |= entry.bits[j].bit;
            }
        }
        if (bits) {
            _writeRamMasked(set ? bits : 0x00, entry.address, bits);
        }
    }
}

void SegLCD_HT1621_LCM59011::_updateLabels(uint8_t labels, bool set) {
    if (labels & LABEL_MCB) {
        _writeRamMasked(set ? LABEL_BIT_MCB : 0x00, 0x00, LABEL_BIT_MCB);
    }
}

void SegLCD_HT1621_LCM59011::_updateSymbols(uint32_t symbols, bool set) {
    size_t count;
    const AddressMapping* map = _getSymbolMap(count);
    for (size_t i = 0; i < count; i++) {
        const AddressMapping& entry = map[i];
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

void SegLCD_HT1621_LCM59011::setUnits(uint8_t units) {
    _updateUnits(units, true);
}

void SegLCD_HT1621_LCM59011::clearUnits(uint8_t units) {
    _updateUnits(units, false);
}

void SegLCD_HT1621_LCM59011::setLabels(uint8_t labels) {
    _updateLabels(labels, true);
}

void SegLCD_HT1621_LCM59011::clearLabels(uint8_t labels) {
    _updateLabels(labels, false);
}

void SegLCD_HT1621_LCM59011::setSymbols(uint32_t symbols) {
    _updateSymbols(symbols, true);
}

void SegLCD_HT1621_LCM59011::clearSymbols(uint32_t symbols) {
    _updateSymbols(symbols, false);
}

void SegLCD_HT1621_LCM59011::setWarningSymbol(bool state, uint8_t index) {
    if (index > 0) return;
    _writeRamMasked(state ? SYMBOL_BIT_WARNING : 0x00, 0x0C, SYMBOL_BIT_WARNING);
}

void SegLCD_HT1621_LCM59011::_setColon(uint8_t row, uint8_t col, bool state) {
    (void)row;
    (void)col;
    _writeRamMasked(state ? COLON_BIT : 0x00, COLON_ADDRESS, COLON_BIT);
}

size_t SegLCD_HT1621_LCM59011::write(uint8_t ch) {
    if (_cursorCol < 0 || _cursorCol >= DIGITS) {
        return 0;
    }

    // Handle colon: ':' at col 2 (between digit 2 and 3)
    if (ch == ':' && _cursorCol == COLON_COL) {
        if (!_isFlagSet(FLAG_COLON_DISPLAYED)) {
            _setColon(_cursorRow, COLON_COL, true);
            _setFlag(FLAG_COLON_DISPLAYED);
        }
        return 1;
    }

    // Clear colon when rewriting digit at col before colon
    if (ch != ':' && _cursorCol == (COLON_COL - 1)) {
        _setColon(_cursorRow, COLON_COL, false);
        _clearFlag(FLAG_COLON_DISPLAYED);
    }

    uint8_t segment_data = _mapSegments(_get_char_value(ch));
    uint8_t hw_addr = _cursorCol * 2;

    _writeRamMasked(segment_data, hw_addr, 0x7F);

    _cursorCol++;
    return 1;
}

// ABCD_EFGH to _EGF_DCBA
uint8_t SegLCD_HT1621_LCM59011::_mapSegments(uint8_t val) {
    uint8_t out = 0;

    out |= (val & 0b10000000) >> 7;  // A: bit7 → bit0
    out |= (val & 0b01000000) >> 5;  // B: bit6 → bit1
    out |= (val & 0b00100000) >> 3;  // C: bit5 → bit2
    out |= (val & 0b00010000) >> 1;  // D: bit4 → bit3
    out |= (val & 0b00001000) << 3;  // E: bit3 → bit6
    out |= (val & 0b00000100) << 2;  // F: bit2 → bit4
    out |= (val & 0b00000010) << 4;  // G: bit1 → bit5

    return out;
}
