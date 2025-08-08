#ifndef SEGDRIVER_3WIRE_H
#define SEGDRIVER_3WIRE_H

#include <SegLCDLib.h>

/**
 * @brief Base class for 3-wire serial LCD segment display drivers.
 * 
 * This class implements the common 3-wire serial communication protocol
 * used by controllers like HT1621, VK0192, and HT1622.
 */
class SegDriver_3Wire : public SegLCDLib {
    // Define operation codes
    #define OP_CMD       0b100
    #define OP_READ      0b110
    #define OP_WRITE     0b101
    #define OP_RD_MOD_WR 0b101

    // System control commands
    #define CMD_SYS_DIS  0b00000000  ///< System disable command (default)
    #define CMD_SYS_EN   0b00000001  ///< System enable command
    #define CMD_LCD_OFF  0b00000010  ///< LCD off command (default)
    #define CMD_LCD_ON   0b00000011  ///< LCD on command
    #define CMD_TMR_DIS  0b00000100  ///< Timer disable command
    #define CMD_WDT_DIS  0b00000101  ///< Watchdog timer disable command
    #define CMD_TMR_EN   0b00000110  ///< Timer enable command
    #define CMD_WDT_EN   0b00000111  ///< Watchdog timer enable command
    #define CMD_CLR_TMR  0b00001100  ///< Clear timer command
    #define CMD_CLR_WDT  0b00001110  ///< Clear watchdog timer command

    // IRQ control commands
    #define CMD_IRQ_DIS  0b10000000  ///< Disable IRQ (default)
    #define CMD_IRQ_EN   0b10001000  ///< Enable IRQ

    // Time base and WDT frequency commands
    #define CMD_F1       0b10100000  ///< Time base/WDT clock output: 1Hz, WDT flag 4sec
    #define CMD_F2       0b10100001  ///< Time base/WDT clock output: 2Hz, WDT flag 2sec
    #define CMD_F4       0b10100010  ///< Time base/WDT clock output: 4Hz, WDT flag 1sec
    #define CMD_F8       0b10100011  ///< Time base/WDT clock output: 8Hz, WDT flag 0.5sec
    #define CMD_F16      0b10100100  ///< Time base/WDT clock output: 16Hz, WDT flag 0.25sec
    #define CMD_F32      0b10100101  ///< Time base/WDT clock output: 32Hz, WDT flag 0.125sec
    #define CMD_F64      0b10100110  ///< Time base/WDT clock output: 64Hz, WDT flag 0.0625sec
    #define CMD_F128     0b10100111  ///< Time base/WDT clock output: 128Hz, WDT flag 0.03125sec (default)

    // Test/Normal operation commands
    #define CMD_TEST     0b11100000  ///< Test command, user don't use.
    #define CMD_NORMAL   0b11100011  ///< Normal operation (default)

    public:
        /**
         * @brief Constructor for 3-wire serial LCD driver.
         * 
         * @param chipselect Chip select pin
         * @param data Data pin
         * @param write Write clock pin
         * @param read Read clock pin (optional, default -1)
         */
        SegDriver_3Wire(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);

        /**
         * @brief Initialize GPIO pins for communication.
         */
        void init() override;

        /**
         * @brief Turn the display on.
         */
        void on() override;

        /**
         * @brief Turn the display off.
         */
        void off() override;

        /**
         * @brief Send command to the controller.
         * 
         * @param command Command byte to send
         */
        void command(uint8_t command) override;

    protected:
        uint8_t _wr;   ///< Write clock pin for the display
        uint8_t _rd;   ///< Read clock pin for the display
        uint8_t _data; ///< Data pin for the display
        uint8_t _cs;   ///< Chip select pin for the display
        uint8_t _irq;  ///< Not used, but reserved for future use
        
        uint8_t _maxAddress; ///< Maximum address for this controller

        /**
         * @brief Write single byte to RAM at specified address.
         * 
         * @param data Data byte to write
         * @param address RAM address (0-based)
         */
        void _writeRam(uint8_t data, uint8_t address);

        /**
         * @brief Write multiple bytes to RAM starting at specified address.
         * 
         * @param data Pointer to data array
         * @param length Number of bytes to write
         * @param address Starting RAM address (0-based)
         */
        virtual void _writeRam(uint8_t *data, size_t length, uint8_t address);

        /**
         * @brief Send bits to the controller.
         * 
         * This method can be overridden by derived classes to implement
         * controller-specific timing requirements.
         * 
         * @param data Data to send
         * @param bitCount Number of bits to send (default 8)
         */
        virtual void _sendBits(uint16_t data, uint8_t bitCount = 8);
};

#endif