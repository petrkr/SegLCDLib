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

        SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void clear() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setProgress(uint8_t value);
        void setWheel(uint16_t value);
        void setLabels(LabelFlags labels);
        void clearLabels(LabelFlags labels);
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col);
        size_t write(uint8_t ch) override;

    private:
        uint8_t _buffer_sigbatt = 0;
        uint8_t _buffer_labels = 0;

        uint8_t _buffer_top[4] = {0};
        uint8_t _buffer_default[6] = {0};

        bool _colon_top = false;
        bool _colon_default = false;

        static constexpr uint8_t DECIMAL_POINT_BIT = 0x10;

        uint8_t _mapSegments(uint8_t val);
};
#endif
