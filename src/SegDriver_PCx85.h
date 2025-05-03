#ifndef SEGDRIVER_PCx85_H
#define SEGDRIVER_PCx85_H

#include <Wire.h>
#include <SegLCDLib.h>

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
        // Define enums for the modes
        typedef enum {
            MODE_STATUS_BLANK = 0,
            MODE_STATUS_ENABLED = 1
        } ModeStatus;

        typedef enum {
            BLINK_FREQUENCY_OFF = 0,
            BLINK_FREQUENCY_1 = 1,
            BLINK_FREQUENCY_2 = 2,
            BLINK_FREQUENCY_3 = 3
        } BlinkFrequency;

        typedef enum {
            BLINK_MODE_NORMAL = 0,
            BLINK_MODE_ALTRAM = 1
        } BlinkMode;

        SegDriver_PCx85(TwoWire& i2c, uint8_t address, uint8_t subaddress);
        virtual void begin() override;
        void bankSelect(uint8_t input, uint8_t output);
        void blink(BlinkFrequency blink = BLINK_FREQUENCY_OFF, BlinkMode mode = BLINK_MODE_NORMAL);
        void clear() override;
        void display() override;
        void noDisplay() override;

    protected:
        void _setMode(ModeStatus status, ModeDrive drive = MODE_DRIVE_14 , ModeBias bias = MODE_BIAS_13);
        void _write(uint8_t data, uint8_t address = 0) override;
        void _write(uint8_t *data, size_t length, uint8_t address = 0) override;

    private:
        TwoWire& _i2c;
        uint8_t _address;
        uint8_t _subaddress;
        ModeDrive _drive;
        ModeBias _bias;

        void _deviceSelect();
};

class SegDriver_PCF85176 : public SegDriver_PCx85 {
    // Maximum possible address for the PCF85176
    #define MAX_HW_ADDRESS  39
    #define MAX_ADDRESS     MAX_HW_ADDRESS

    public:
        SegDriver_PCF85176(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0) : SegDriver_PCx85(i2c, address, subaddress) {}
};

#endif
