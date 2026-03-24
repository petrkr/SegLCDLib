#include <Arduino.h>
#include <SegTransport.h>

SegTransport3WireArduino::SegTransport3WireArduino(uint8_t data, uint8_t write, uint8_t read)
    : _data(data), _write(write), _read(read) {
    pinMode(_data, OUTPUT);
    pinMode(_write, OUTPUT);
    if (_read != static_cast<uint8_t>(-1)) {
        pinMode(_read, OUTPUT);
    }
}

void SegTransport3WireArduino::set_cs(uint8_t chipselect, bool state) {
    delayMicroseconds(1);
    digitalWrite(chipselect, state);
    delayMicroseconds(1);
}

void SegTransport3WireArduino::write(uint16_t data, uint8_t bitCount) {
    for (int8_t i = bitCount - 1; i >= 0; i--) {
        digitalWrite(_data, (data >> i) & 1);
        delayMicroseconds(1);
        digitalWrite(_write, LOW);
        delayMicroseconds(4);
        digitalWrite(_write, HIGH);
        delayMicroseconds(4);
    }
}

SegTransportI2CArduino::SegTransportI2CArduino(TwoWire& i2c) : _i2c(i2c) { }

void SegTransportI2CArduino::write(uint8_t address, uint8_t data) {
    _i2c.beginTransmission(address);
    _i2c.write(data);
    _i2c.endTransmission();
}

void SegTransportI2CArduino::write(uint8_t address, uint8_t *data, size_t length) {
    _i2c.beginTransmission(address);
    _i2c.write(data, length);
    _i2c.endTransmission();
}
