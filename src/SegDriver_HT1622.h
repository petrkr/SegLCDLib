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
    // HT1622 specific commands (not in base class)
    #define CMD_TONE_OFF 0b00001000  ///< Tone off command (default)

    #define CMD_RC_32K   0b00011000  ///< System clock source, internal RC oscillator (default)
    #define CMD_EXT_32K  0b00011100  ///< System clock source, external RC oscillator

    #define CMD_TONE_4K  0b01000000  ///< Tone frequency 4kHz
    #define CMD_TONE_2K  0b01100000  ///< Tone frequency 2kHz

    public:
        static constexpr uint8_t MAX_ADDRESS = 62; ///< Last valid start HW nibble address for byte write

        /**
         * @brief Constructor for HT1622 segment driver
         *
         * @param transport 3-wire transport implementation
         * @param chipselect Chip select pin for the display
         */
        SegDriver_HT1622(SegTransport3Wire& transport, uint8_t chipselect);
};

#endif
