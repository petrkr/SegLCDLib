#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_TempHumidity : public SegDriver_PCF85176 {
    #define MAX_ADDRESS 31
    
    #define ADDR_SIGNAL_BATT 14
    #define ADDR_TEMP_SEGS 0
    #define ADDR_HUM_SEGS 8

    public:
        typedef enum {
            LABEL_C = 0x01,
            LABEL_PROC = 0x02,
        } LabelFlags;

        SegLCD_PCF85176_TempHumidity(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0);
        void init() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setLabels(uint8_t labels);
        void clearLabels(uint8_t labels);
        void setDecimal(uint8_t digit, bool state, LCDSections section = LCDSections::SECTION_DEFAULT);
        void writeFloat(float input, uint8_t decimals = 0, LCDSections section = LCDSections::SECTION_DEFAULT);
        void writeChar(uint8_t digit, char c, LCDSections section = LCDSections::SECTION_DEFAULT) override;

    private:
        uint8_t _buffer_sigbatt = 0;

        uint8_t _buffer_temp[4] = {0};
        uint8_t _buffer_hum[3] = {0};

        uint8_t _mapSegments(uint8_t val);
};
#endif
