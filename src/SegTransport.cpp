#include <Arduino.h>
#include <SegTransport.h>


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
