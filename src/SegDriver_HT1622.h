#ifndef SEGDRIVER_HT1622_H
#define SEGDRIVER_HT1622_H

#include "SegDriver_3Wire.h"

/**
 * @brief HT1622 LCD segment display driver.
 *
 * This driver extends the 3-wire base driver with HT1622-specific features
 * and stricter timing requirements.
 */
class SegDriver_HT1622 : public SegDriver_3Wire {
    #define MAX_HW_ADDRESS  62
    #define MAX_ADDRESS     62

    // HT1622 specific commands (not in base class)
    #define CMD_TONE_OFF 0b00001000  ///< Tone off command (default)

    #define CMD_RC_32K   0b00011000  ///< System clock source, internal RC oscillator (default)
    #define CMD_EXT_32K  0b00011100  ///< System clock source, external RC oscillator

    #define CMD_TONE_4K  0b01000000  ///< Tone frequency 4kHz
    #define CMD_TONE_2K  0b01100000  ///< Tone frequency 2kHz

    public:
        /**
         * @brief Constructor for HT1622 segment driver
         *
         * @param chipselect Chip select pin for the display
         * @param data Data pin for the display
         * @param write Write clock pin for the display
         * @param read Read clock pin for the display (optional, default is -1)
         */
        SegDriver_HT1622(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);

        void clear() override;

    protected:
        /**
         * @brief Override _writeRam to implement HT1622-specific timing requirements.
         */
        void _writeRam(uint8_t data, uint8_t address = 0);
        void _writeRam(uint8_t *data, size_t length, uint8_t address = 0) override;

        /**
         * @brief Override _sendBits to implement HT1622-specific timing requirements.
         */
        void _sendBits(uint16_t data, uint8_t bitCount = 8) override;
};

#endif
