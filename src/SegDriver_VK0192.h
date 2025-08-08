#ifndef SEGDRIVER_VK0192_H
#define SEGDRIVER_VK0192_H

#include "SegDriver_3Wire.h"

/**
 * @brief VK0192 LCD segment display driver.
 *
 * This driver extends the 3-wire base driver with VK0192-specific features
 * and stricter timing requirements.
 */
class SegDriver_VK0192 : public SegDriver_3Wire {
    #define MAX_HW_ADDRESS  47
    #define MAX_ADDRESS     47

    // VK0192 specific commands (not in base class)
    #define CMD_RC_32K   0b00011000  ///< System clock source, internal RC oscillator (default)
    #define CMD_EXT_32K  0b00011100  ///< System clock source, external RC oscillator

    public:
        /**
         * @brief Constructor for VK0192 segment driver
         *
         * @param chipselect Chip select pin for the display
         * @param data Data pin for the display
         * @param write Write clock pin for the display
         * @param read Read clock pin for the display (optional, default is -1)
         */
        SegDriver_VK0192(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);

        void clear() override;

    protected:
        /**
         * @brief Override _writeRam to implement VK0192-specific timing requirements.
         */
        void _writeRam(uint8_t data, uint8_t address = 0);
        void _writeRam(uint8_t *data, size_t length, uint8_t address = 0) override;

        /**
         * @brief Override _sendBits to implement VK0192-specific timing requirements.
         */
        void _sendBits(uint16_t data, uint8_t bitCount = 8) override;
};

#endif
