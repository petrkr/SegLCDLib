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
    // VK0192 specific commands (not in base class)
    #define CMD_RC_32K   0b00011000  ///< System clock source, internal RC oscillator (default)
    #define CMD_EXT_32K  0b00011100  ///< System clock source, external RC oscillator

    public:
        static constexpr uint8_t MAX_ADDRESS = 46; ///< Last valid start HW nibble address for byte write

        /**
         * @brief Constructor for VK0192 segment driver
         *
         * @param chipselect Chip select pin for the display
         * @param data Data pin for the display
         * @param write Write clock pin for the display
         * @param read Read clock pin for the display (optional, default is -1)
         */
        SegDriver_VK0192(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);

        /**
         * @brief Flush specific range of buffered changes to the display.
         *
         * Optimized implementation for VK0192 that loops through each address
         * since the 3-wire protocol doesn't support bulk write.
         *
         * @param startAddr Starting buffer address in bytes (RAM buffer index)
         * @param length Number of bytes to flush
         */
        void flush(uint8_t startAddr, uint8_t length) override;
        using SegLCDLib::flush;  // Inherit base flush() without parameters
};

#endif
