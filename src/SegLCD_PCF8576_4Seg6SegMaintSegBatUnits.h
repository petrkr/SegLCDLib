#ifndef SEGLCD_PCF8576_4SEG6SEGMAINTSEGBATUNITS_H
#define SEGLCD_PCF8576_4SEG6SEGMAINTSEGBATUNITS_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF8576_4Seg6SegMaintSegBatUnits : public SegDriver_PCF8576 {

    #define MAX_ADDRESS 24

    #define ADDR_SIGNAL 0x0A
    #define ADDR_BATT 0x08
    #define ADDR_SMALL_SEGS 0x00
    #define ADDR_LABELS 0x11
    #define ADDR_BIG_SEGS 0x0B

    public:
        typedef enum {
            LABEL_MPA = 0x01,
            LABEL_KPA = 0x02,
            LABEL_UEPS = 0x04,
            LABEL_MM = 0x08,
            LABEL_MV = 0x10,
            LABEL_NM = 0x20,
            LABEL_KG = 0x40,
            LABEL_G = 0x80,
            LABEL_ALL = 0xFF
        } LabelFlags;

        SegLCD_PCF8576_4Seg6SegMaintSegBatUnits(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void clear() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setLabels(uint8_t labels);
        void clearLabels(uint8_t labels);
        void setDegree(bool state);
        void setMaintenance(bool state);
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col);
        size_t write(uint8_t ch) override;

    private:
        static constexpr uint8_t RAM_ADDR_LAST = 0x18;
        static constexpr uint8_t RAM_BYTE_COUNT = (RAM_ADDR_LAST + 2) / 2;

        uint8_t _buffer_default[6] = {0};

        bool _colon_top = false;
        bool _colon_default_left = false;
        bool _colon_default_right = false;

        uint8_t _mapSegmentsTop(uint8_t val);
        uint8_t _mapSegments(uint8_t val);
        uint8_t _convertLabelFlags(uint8_t labels);
        size_t _writeRow0(uint8_t ch);
        size_t _writeRow1(uint8_t ch);
};
#endif
