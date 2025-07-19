#ifndef SEGDRIVER_PCx85_H
#define SEGDRIVER_PCx85_H

#include <Wire.h>
#include <SegLCDLib.h>

/**
 * @brief Base class for PCx85 LCD segment display drivers.
 *
 * Implementation of the PCF85176 and similar I2C controllers.
 */
class SegDriver_PCx85 : public SegLCDLib {
    #define MAX_HW_ADDRESS  0
    #define MAX_ADDRESS     0

    // Define commands
    #define CMD_LOAD_POINTER    0x00
    #define CMD_MODE            0x40
    #define CMD_DEVICE_SELECT   0x60
    #define CMD_BLINK           0x70
    #define CMD_BANK_SELECT     0x78
    #define CMD_LAST_COMMAND    0x80

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
         * @param i2c Reference to the TwoWire object for I2C communication
         * @param address I2C address of the PCx85 device
         * @param subaddress Subaddress for the device
         */
        SegDriver_PCx85(TwoWire& i2c, uint8_t address, uint8_t subaddress);
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
        void clear() override;
        void on() override;
        void off() override;

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
        void _write(uint8_t data, uint8_t address = 0) override;
        void _write(uint8_t *data, size_t length, uint8_t address = 0) override;

    private:
        TwoWire& _i2c;
        uint8_t _address;
        uint8_t _subaddress;
        ModeDrive _drive;
        ModeBias _bias;

        /**
         * @brief Select the device for communication.
         *
         * This method is used to set the device by subaddress.
         * If address during writing overflow one device's address
         * next device is selected. So this is used to select
         * back to this device.
         */
        void _deviceSelect();
};

/**
 * @brief Implementation of the PCF85176 controllers.
 */
class SegDriver_PCF85176 : public SegDriver_PCx85 {
    // TODO: make this somehow better?
    #define MAX_HW_ADDRESS  39              ///< Maximum hardware address for PCF85176
    #define MAX_ADDRESS     MAX_HW_ADDRESS  ///< Maximum usable address for LCD implementation

    public:
        /**
         * @brief Constructor for PCF85176 segment driver
         *
         * @param i2c Reference to the TwoWire object for I2C communication
         * @param address I2C address of the PCF85176 device (default is 0x38)
         * @param subaddress Subaddress for the device (default is 0)
         */
        SegDriver_PCF85176(TwoWire& i2c, uint8_t address = 0x38, uint8_t subaddress = 0) : SegDriver_PCx85(i2c, address, subaddress) {}
};

#endif
