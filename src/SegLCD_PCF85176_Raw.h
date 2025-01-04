#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_Raw : public SegDriver_PCF85176 {
    public:
        SegLCD_PCF85176_Raw(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0) : SegDriver_PCF85176(i2c, address, subaddress) {}
        void begin(ModeBias bias = MODE_BIAS_13, ModeDrive drive = MODE_DRIVE_14) {
            SegDriver_PCF85176::begin();
            _setMode(MODE_STATUS_ENABLED, bias, drive);
        }

        // For generic class allow access to low level functions
        void write(uint8_t data, uint8_t address = 0) {
            _write(data, address);
        }

        void write(uint8_t *data, size_t length, uint8_t address = 0) {
            _write(data, length, address);
        }

};
#endif
