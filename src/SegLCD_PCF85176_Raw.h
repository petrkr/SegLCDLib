#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_Raw : public SegDriver_PCF85176 {
    public:
        SegLCD_PCF85176_Raw(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init(ModeDrive drive, ModeBias bias);
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
