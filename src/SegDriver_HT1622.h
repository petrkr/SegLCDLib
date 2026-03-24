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
         * @param chipselect Chip select pin for the display
         * @param data Data pin for the display
         * @param write Write clock pin for the display
         * @param read Read clock pin for the display (optional, default is -1)
         */
        SegDriver_HT1622(SegTransport3Wire& transport, uint8_t chipselect);

        /**
         * @brief Flush specific range of buffered changes to the display.
         *
         * Optimized implementation for HT1622 that loops through each address
         * since the 3-wire protocol doesn't support bulk write.
         *
         * @param startAddr Starting buffer address in bytes (RAM buffer index)
         * @param length Number of bytes to flush
         */
        void flush(uint8_t startAddr, uint8_t length) override;
        using SegLCDLib::flush;  // Inherit base flush() without parameters
};

#endif
