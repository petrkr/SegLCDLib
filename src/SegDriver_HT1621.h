#ifndef SEGDRIVER_HT1621_H
#define SEGDRIVER_HT1621_H

#include "SegDriver_3Wire.h"

/**
 * @brief HT1621 LCD segment display driver.
 *
 * This driver extends the 3-wire base driver with HT1621-specific features
 * including TONE commands and configurable BIAS/DRIVE modes.
 */
class SegDriver_HT1621 : public SegDriver_3Wire {
    #define MAX_HW_ADDRESS  30
    #define MAX_ADDRESS     30

    // HT1621 specific commands
    #define CMD_TONE_OFF 0b00001000  ///< Tone off command (default)
    #define CMD_TONE_ON  0b00001001  ///< Tone on command
    #define CMD_XTAL_32K 0b00010100  ///< System clock source, crystal oscillator
    #define CMD_RC_256K  0b00011000  ///< System clock source, internal RC oscillator (default)
    #define CMD_EXT_256K 0b00011100  ///< System clock source, external RC oscillator

    #define CMD_BIAS_12  0b00100000  ///< Bias mode 1/2, bits 2,3 select common options 00: 2COM, 01: 3COM, 10: 4COM
    #define CMD_BIAS_13  0b00100001  ///< Bias mode 1/3, bits 2,3 select common options 00: 2COM, 01: 3COM, 10: 4COM

    #define CMD_TONE_4K  0b01000000  ///< Tone frequency 4kHz
    #define CMD_TONE_2K  0b01100000  ///< Tone frequency 2kHz

    #define DRIVE_MODE_12_MASK 0b00000000 ///< 1/2 multiplexing drive mode
    #define DRIVE_MODE_13_MASK 0b00000100 ///< 1/3 multiplexing drive mode
    #define DRIVE_MODE_14_MASK 0b00001000 ///< 1/4 multiplexing drive mode

    public:
        /**
         * @brief Constructor for HT1621 segment driver
         *
         * @param chipselect Chip select pin for the display
         * @param data Data pin for the display
         * @param write Write clock pin for the display
         * @param read Read clock pin for the display (optional, default is -1)
         */
        SegDriver_HT1621(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);

        void clear() override;

    protected:
        /**
         * @brief Set the mode of the display.
         *
         * This method sets the drive and bias modes for the display.
         *
         * @param drive Drive mode (default is MODE_DRIVE_14)
         * @param bias Bias mode (default is MODE_BIAS_13)
         */
        void _setMode(ModeDrive drive = MODE_DRIVE_14 , ModeBias bias = MODE_BIAS_13);

    private:
        ModeDrive _drive; ///< Current drive mode

};

#endif
