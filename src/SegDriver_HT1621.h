#ifndef SEGDRIVER_HT1621_H
#define SEGDRIVER_HT1621_H

#include <SegLCDLib.h>

/**
 * @brief Base class for HT1621 LCD segment display driver.
 */
class SegDriver_HT1621 : public SegLCDLib {
    #define MAX_HW_ADDRESS  30
    #define MAX_ADDRESS     30

    // Define commands
    #define OP_CMD       0b100
    #define OP_READ      0b110
    #define OP_WRITE     0b101
    #define OP_RD_MOD_WR 0b101

    #define CMD_SYS_DIS  0b00000000  ///< System disable command (default)
    #define CMD_SYS_EN   0b00000001  ///< System enable command
    #define CMD_LCD_OFF  0b00000010  ///< LCD off command (default)
    #define CMD_LCD_ON   0b00000011  ///< LCD on command
    #define CMD_TMR_DIS  0b00000100  ///< Timer disable command
    #define CMD_WDT_DIS  0b00000101  ///< Watchdog timer disable command
    #define CMD_TMR_EN   0b00000110  ///< Timer enable command
    #define CMD_WDT_EN   0b00000111  ///< Watchdog timer enable command
    #define CMD_TONE_OFF 0b00001000  ///< Tone off command (default)
    #define CMD_TONE_ON  0b00001001  ///< Tone on command
    #define CMD_CLR_TMR  0b00001100  ///< Clear timer command
    #define CMD_CLR_WDT  0b00001110  ///< Clear watchdog timer command
    #define CMD_XTAL_32K 0b00010100  ///< System clock source, crystal oscillator
    #define CMD_RC_256K  0b00011000  ///< System clock source, internal RC oscillator (default)
    #define CMD_EXT_256K 0b00011100  ///< System clock source, external RC oscillator

    #define CMD_BIAS_12  0b00100000  ///< Bias mode 1/2, bits 2,3 select common options 00: 2COM, 01: 3COM, 10: 4COM
    #define CMD_BIAS_13  0b00100001  ///< Bias mode 1/3, bits 2,3 select common options 00: 2COM, 01: 3COM, 10: 4COM

    #define CMD_TONE_4K  0b01000000  ///< Tone frequency 4kHz
    #define CMD_TONE_2K  0b01100000  ///< Tone frequency 2kHz

    #define CMD_IRQ_DIS  0b10000000  ///< Disable IRQ (default)
    #define CMD_IRQ_EN   0b10001000  ///< Enable IRQ

    #define CMD_F1       0b10100000  ///< Time base/WDT clock output: 1Hz, WDT flag 4sec
    #define CMD_F2       0b10100001  ///< Time base/WDT clock output: 2Hz, WDT flag 2sec
    #define CMD_F4       0b10100010  ///< Time base/WDT clock output: 4Hz, WDT flag 1sec
    #define CMD_F8       0b10100011  ///< Time base/WDT clock output: 8Hz, WDT flag 0.5sec
    #define CMD_F16      0b10100100  ///< Time base/WDT clock output: 16Hz, WDT flag 0.25sec
    #define CMD_F32      0b10100101  ///< Time base/WDT clock output: 32Hz, WDT flag 0.125sec
    #define CMD_F64      0b10100110  ///< Time base/WDT clock output: 64Hz, WDT flag 0.0625sec
    #define CMD_F128     0b10100111  ///< Time base/WDT clock output: 128Hz, WDT flag 0.03125sec (default)

    #define CMD_TEST     0b11100000  ///< Test command, user don't use.
    #define CMD_NORMAL   0b11100011  ///< Normal operation (default)

    #define DRIVE_MODE_12_MASK 0b00000000 ///< 1/2 multiplexing drive mode
    #define DRIVE_MODE_13_MASK 0b00000100 ///< 1/3 multiplexing drive mode
    #define DRIVE_MODE_14_MASK 0b00001000 ///< 1/4 multiplexing drive mode

    public:
        /**
         * @brief Constructor for PCx85 segment driver
         *
         * Address is selected by SA0 pins on the device.
         * Subaddress is selected by A0 to A2 pins on the device
         *
         * @param chipselect Chip select pin for the display
         * @param data Data pin for the display
         * @param write Write clock pin for the display
         * @param read Read clock pin for the display (optional, default is -1)
         */
        SegDriver_HT1621(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        virtual void init() override;

        void clear() override;
        void on() override;
        void off() override;

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
        void _write(uint8_t data, uint8_t address = 0) override;
        void _write(uint8_t *data, size_t length, uint8_t address = 0) override;

        /**
         * @brief Send a command to the display.
         *
         * This method sends a command to the display using the write clock and data pins.
         *
         * @param command The command to send
         */
        void _sendCommand(uint8_t command);

    private:
        uint8_t _wr;   ///< Write clock pin for the display
        uint8_t _rd;   ///< Read clock pin for the display
        uint8_t _data; ///< Data pin for the display
        uint8_t _cs;   ///< Chip select pin for the display
        uint8_t _irq;  ///< Not used, but reserved for future use
        ModeDrive _drive; ///< Current drive mode

        void _sendBits(uint16_t data, uint8_t bitCount = 8);


};

#endif
