#ifndef SEGDRIVER_PCx85_H
#define SEGDRIVER_PCx85_H

#include <SegLCDLib.h>
#include <SegTransport.h>

/**
 * @brief Base class for PCx85 LCD segment display drivers.
 *
 * Shared behavior for PCx85 LCD segment display drivers.
 */
class SegDriver_PCx85 : public SegLCDLib {
    protected:
        static constexpr uint8_t CMD_MODE            = 0x40;
        static constexpr uint8_t CMD_DEVICE_SELECT   = 0x60;
        static constexpr uint8_t CMD_BLINK           = 0x70;
        static constexpr uint8_t CMD_BANK_SELECT     = 0x78;
        static constexpr uint8_t DEFAULT_PCF85176_I2C_ADDRESS = 0x38;  // 56 decimal
        static constexpr uint8_t DEFAULT_SUBADDRESS = 0x00;

    public:
        /**
         * @brief Enable/disable display
         */
        typedef enum {
            MODE_STATUS_BLANK = 0,   ///< Display is blanked
            MODE_STATUS_ENABLED = 1  ///< Display is enabled
        } ModeStatus;

        /**
         * @brief Set blink frequency
         */
        typedef enum {
            BLINK_FREQUENCY_OFF = 0, ///< No blinking
            BLINK_FREQUENCY_1 = 1,   ///< Blink at ~2Hz
            BLINK_FREQUENCY_2 = 2,   ///< Blink at ~1Hz
            BLINK_FREQUENCY_3 = 3    ///< Blink at ~0.5Hz
        } BlinkFrequency;

        /**
         * @brief Set blink mode
         *
         * Note: that alternate RAM blink mode is not supported
         * if 1/3 or 1/4 drive mode is used.
         */
        typedef enum {
            BLINK_MODE_NORMAL = 0,  ///< Normal blink mode
            BLINK_MODE_ALTRAM = 1   ///< Alternate RAM blink mode
        } BlinkMode;

        /**
         * @brief Constructor for PCx85 segment driver
         *
         * Address is selected by SA0 pins on the device.
         * Subaddress is selected by A0 to A2 pins on the device
         *
         * @param transport I2C transport implementation
         * @param address I2C address of the PCx85 device
         * @param subaddress Subaddress for the device
         */
        SegDriver_PCx85(SegTransportI2C& transport, uint8_t address, uint8_t subaddress);
        virtual void init() override;

        /**
         * @brief Select bank for input and output.
         *
         * The bank-select command controls where data is written to
         * RAM and where it is displayed from.
         * Valid values for input and output are 0 or 1.
         * @param input input bank selection; storage of arriving display data
         * @param output output bank selection; retrieval of LCD display data
         */
        void bankSelect(uint8_t input, uint8_t output);

        /**
         * @brief Set blink frequency and mode.
         *
         * @param blink Blink frequency to set (default is BLINK_FREQUENCY_OFF)
         * @param mode Blink mode to set (default is BLINK_MODE_NORMAL)
         */
        void blink(BlinkFrequency blink = BLINK_FREQUENCY_OFF, BlinkMode mode = BLINK_MODE_NORMAL);
        void on() override;
        void off() override;

        void command(uint8_t command) override;

        /**
         * @brief Flush specific range of buffered changes to the display.
         *
         * Optimized implementation for PCx85 that uses efficient I2C bulk write.
         *
         * @param startAddr Starting buffer address in bytes (RAM buffer index)
         * @param length Number of bytes to flush
         */
        void flush(uint8_t startAddr, uint8_t length) override;
        using SegLCDLib::flush;  // Inherit base flush() without parameters

    protected:
        /**
         * @brief Set the mode of the display.
         *
         * This method sets the drive and bias modes for the display.
         * It also enables or disables the display based on the status.
         *
         * @param status Display status (enabled or blanked)
         * @param drive Drive mode (default is MODE_DRIVE_14)
         * @param bias Bias mode (default is MODE_BIAS_13)
         */
        void _setMode(ModeStatus status, ModeDrive drive = MODE_DRIVE_14 , ModeBias bias = MODE_BIAS_13);
        using SegLCDLib::_writeRam;
        void _writeRam(uint8_t *data, size_t length, uint8_t address = 0) override = 0;

        virtual void _sendCommand(uint8_t command, bool last = true) = 0;

        SegTransportI2C& _transport;
        uint8_t _address;
        uint8_t _subaddress;
        ModeDrive _drive;
        ModeBias _bias;
};

#endif
