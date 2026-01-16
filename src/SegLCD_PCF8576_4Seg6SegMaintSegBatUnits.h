#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF8576_4Seg6SegMaintSegBatUnits : public SegDriver_PCF8576 {

    #define MAX_ADDRESS 24

    #define ADDR_SIGNAL 0x0A
    #define ADDR_BATT 0x08
    #define ADDR_SMALL_SEGS 0
    #define ADDR_LABELS 17
    #define ADDR_BIG_SEGS 11

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

        SegLCD_PCF8576_4Seg6SegMaintSegBatUnits(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void clear() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setLabels(LabelFlags labels);
        void clearLabels(LabelFlags labels);
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col);
        size_t write(uint8_t ch) override;

        // TODO: Remove after implementation
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);

    private:
        static constexpr uint8_t RAM_ADDR_LAST = 0x18;
        static constexpr uint8_t RAM_BYTE_COUNT = (RAM_ADDR_LAST + 2) / 2;
        uint8_t _buffer[RAM_BYTE_COUNT] = {0};

        uint8_t _buffer_labels[2] = {0};

        uint8_t _buffer_top[4] = {0};
        uint8_t _buffer_default[6] = {0};

        bool _colon_top = false;
        bool _colon_default = false;

        static constexpr uint8_t DECIMAL_POINT_BIT = 0x80;

        uint8_t _mapSegmentsTop(uint8_t val);
        uint8_t _mapSegments(uint8_t val);
        void _writeRamAtAddr(uint8_t data, uint8_t address, uint8_t mask = 0xFF);
};
#endif
