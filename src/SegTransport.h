#ifndef SEGTRANSPORT_H
#define SEGTRANSPORT_H

#include <Arduino.h>

/**
 * @brief Abstract base class for low-level bus transports.
 */
class SegTransport {

};

/**
 * @brief Abstract 3-wire serial transport.
 *
 * Implementations provide DATA/WR signaling and chip-select timing.
 */
class SegTransport3Wire : public SegTransport {
    public:
        virtual ~SegTransport3Wire() = default;

        /**
         * @brief Initialize chip-select pin as output.
         *
         * @param chipselect CS pin
         */
        virtual void init_cs(uint8_t chipselect) = 0;

        /**
         * @brief Drive chip-select line.
         *
         * @param chipselect CS pin
         * @param state Requested CS state
         */
        virtual void set_cs(uint8_t chipselect, bool state) = 0;

        /**
         * @brief Write bits to the 3-wire bus, MSB first.
         *
         * @param data Value to write
         * @param bitCount Number of valid bits in @p data
         */
        virtual void write(uint16_t data, uint8_t bitCount) = 0;
};

/**
 * @brief Abstract I2C transport.
 */
class SegTransportI2C : public SegTransport {
    public:
        virtual ~SegTransportI2C() = default;

        /**
         * @brief Write one byte to an I2C device.
         *
         * @param address I2C address
         * @param data Byte to write
         */
        virtual void write(uint8_t address, uint8_t data) = 0;

        /**
         * @brief Write multiple bytes to an I2C device.
         *
         * @param address I2C address
         * @param data Pointer to data buffer
         * @param length Number of bytes to write
         */
        virtual void write(uint8_t address, uint8_t *data, size_t length) = 0;
};

#endif
