#ifndef SEGTRANSPORTARDUINO_H
#define SEGTRANSPORTARDUINO_H

#ifndef SEGLCD_DISABLE_ARDUINO_TRANSPORT

#include <Arduino.h>
#include <Wire.h>

#include "SegTransport.h"

/**
 * @brief Arduino GPIO implementation of the 3-wire transport.
 */
class SegTransport3WireArduino : public SegTransport3Wire {
    public:
        /**
         * @brief Create Arduino 3-wire transport.
         *
         * @param data DATA pin
         * @param write WR/clock pin
         * @param read Optional RD pin, unused by current drivers
         */
        SegTransport3WireArduino(uint8_t data, uint8_t write, uint8_t read = -1);

        void init_cs(uint8_t chipselect) override;
        void set_cs(uint8_t chipselect, bool state) override;
        void write(uint16_t data, uint8_t bitCount) override;

    private:
        uint8_t _data;
        uint8_t _write;
        uint8_t _read;
};

/**
 * @brief Arduino TwoWire implementation of the I2C transport.
 */
class SegTransportI2CArduino : public SegTransportI2C {
    public:
        /**
         * @brief Create Arduino I2C transport.
         *
         * @param i2c Reference to the TwoWire bus instance
         */
        SegTransportI2CArduino(TwoWire& i2c);
        void write(uint8_t address, uint8_t data) override;
        void write(uint8_t address, uint8_t *data, size_t length) override;

    private:
        TwoWire& _i2c;
};

#endif

#endif
