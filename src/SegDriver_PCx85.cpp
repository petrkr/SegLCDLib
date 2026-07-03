#include <Arduino.h>
#include <SegDriver_PCx85.h>


SegDriver_PCx85::SegDriver_PCx85(SegTransportI2C& transport, uint8_t address, uint8_t subaddress) : _transport(transport) {
    _address = address;
    _subaddress = subaddress;
}

void SegDriver_PCx85::init() {
    _sendCommand(CMD_DEVICE_SELECT | _subaddress);
    SegLCDLib::init();
}

void SegDriver_PCx85::bankSelect(uint8_t input, uint8_t output) {
    _sendCommand(CMD_BANK_SELECT | (input << 1) | output);
}

void SegDriver_PCx85::blink(BlinkFrequency frequency, BlinkMode mode) {
    _sendCommand(CMD_BLINK | (mode << 2) | frequency);
}

void SegDriver_PCx85::on() {
    _setMode(MODE_STATUS_ENABLED, _drive, _bias);
}

void SegDriver_PCx85::off() {
    _setMode(MODE_STATUS_BLANK, _drive, _bias);
}

void SegDriver_PCx85::command(uint8_t command) {
    _sendCommand(command);
}

void SegDriver_PCx85::_setMode(ModeStatus status, ModeDrive drive, ModeBias bias) {
    _drive = drive;
    _bias = bias;

    _sendCommand(CMD_MODE | (status << 3) | (_bias << 2) | _drive);
}

void SegDriver_PCx85::flush(uint8_t startAddr, uint8_t length) {
    if (!_ramBuffer || startAddr >= _ramBufferSize) return;

    bool prevAuto = _autoFlush;
    _autoFlush = true;

    // Clamp length to buffer bounds
    if (startAddr + length > _ramBufferSize) {
        length = _ramBufferSize - startAddr;
    }

    // PCx85 supports efficient bulk write from any address
    // Convert byte index to nibble-level address (startAddr * 2)
    _writeRam(_ramBuffer + startAddr, length, startAddr * 2);

    _autoFlush = prevAuto;
}
