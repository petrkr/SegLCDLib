#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_6DigitSignalBatteryProgress : public SegDriver_PCF85176 {

    #define MAX_ADDRESS 31
    
    #define ADDR_SIGNAL_BATT 0
    #define ADDR_WHEEL 2
    #define ADDR_SMALL_SEGS 6
    #define ADDR_PRES_LABELS 14
    #define ADDR_PROGRESS 16
    #define ADDR_BIG_SEGS 20

    public:
        typedef enum {
            LABEL_MPA = 0x01,
            LABEL_KPA = 0x02,
            LABEL_BAR = 0x04,
            LABEL_PSI = 0x08,
            LABEL_MH2O = 0x10,
            LABEL_MMHG = 0x20,
            LABEL_ATM = 0x40,
            LABEL_KGFCM2 = 0x80,
            LABEL_ALL = 0xFF
        } LabelFlags;

        SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0);
        void begin() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setProgress(uint8_t value);
        void setWheel(uint16_t value);
        void setLabels(LabelFlags labels);
        void clearLabels(LabelFlags labels);
        void setClockColon(bool status, LCDSections section = LCDSections::SECTION_CLOCK);
        void setDecimal(uint8_t digit, bool state, LCDSections section = LCDSections::SECTION_DEFAULT);
        void writeChar(uint8_t digit, char c, LCDSections section = LCDSections::SECTION_DEFAULT) override;

    private:
        uint8_t _buffer_sigbatt = 0;
        uint8_t _buffer_labels = 0;

        uint8_t _buffer_top[4] = {0};
        uint8_t _buffer_default[6] = {0};

        uint8_t _mapSegments(uint8_t val);
};
#endif
