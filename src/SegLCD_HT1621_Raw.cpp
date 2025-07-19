#include <SegLCD_HT1621_Raw.h>


SegLCD_HT1621_Raw::SegLCD_HT1621_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {}

void SegLCD_HT1621_Raw::init(ModeDrive drive, ModeBias bias) {
    SegDriver_HT1621::init();
    _setMode(drive, bias);
}

// For generic class allow access to low level functions
void SegLCD_HT1621_Raw::write(uint8_t data, uint8_t address) {
    _write(data, address);
}

void SegLCD_HT1621_Raw::write(uint8_t *data, size_t length, uint8_t address) {
    _write(data, length, address);
}

void SegLCD_HT1621_Raw::sendCommand(uint8_t command) {
    _sendCommand(command);
}
