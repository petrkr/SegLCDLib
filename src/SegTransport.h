#ifndef SEGTRANSPORT_H
#define SEGTRANSPORT_H

#include <Wire.h>

class SegTransport {

};

class SegTransport3Wire : public SegTransport {
    public:
        virtual ~SegTransport3Wire() = default;
        virtual void set_cs(bool state) = 0;
        virtual void write(uint16_t data, uint8_t bitCount) = 0;
};

class SegTransport3WireArduino : public SegTransport3Wire {
    public:
        SegTransport3WireArduino(uint8_t data, uint8_t write, uint8_t read = -1);

        void set_cs(bool state) override;
        void write(uint16_t data, uint8_t bitCount) override;

    private:
        uint8_t _data;
        uint8_t _write;
        uint8_t _read;
};

/**
 * @brief Implementation of the PCF85176 controllers.
 */
class SegTransportI2C : public SegTransport {
    public:
        virtual ~SegTransportI2C() = default;

        /**
         * @brief Constructor for Arduino TwoWire transport
         *
         * @param i2c Reference to the TwoWire object for I2C communication
         * @param address I2C address
         */
        virtual void write(uint8_t address, uint8_t data) = 0;
        virtual void write(uint8_t address, uint8_t *data, size_t length) = 0;
};

class SegTransportI2CArduino : public SegTransportI2C {
    public:
        SegTransportI2CArduino(TwoWire& i2c);

        /**
         * @brief Constructor for Arduino TwoWire transport
         *
         * @param i2c Reference to the TwoWire object for I2C communication
         * @param address I2C address
         */
        void write(uint8_t address, uint8_t data) override;
        void write(uint8_t address, uint8_t *data, size_t length) override;

    private:
        TwoWire& _i2c;
};

#endif
