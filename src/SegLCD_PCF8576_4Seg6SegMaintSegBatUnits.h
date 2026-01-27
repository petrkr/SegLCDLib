#ifndef SEGLCD_PCF8576_4SEG6SEGMAINTSEGBATUNITS_H
#define SEGLCD_PCF8576_4SEG6SEGMAINTSEGBATUNITS_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @file SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h
 * @brief PCF8576 driver for 4+6 digit LCD with maintenance/battery/signal.
 */

/**
 * @brief 4+6 digit dual LCD with maintenance/battery/signal (PCF8576).
 *
 * Features: maintenance indicator, battery level, signal strength, unit labels.
 * I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 *
 * @see examples/PCF8576/4Seg6SegMaintSegBatUnits/4Seg6SegMaintSegBatUnits.ino
 */
class SegLCD_PCF8576_4Seg6SegMaintSegBatUnits : public SegDriver_PCF8576 {
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
        void setCursor(uint8_t row, uint8_t col);
        size_t write(uint8_t ch) override;

    private:
        void _setColon(uint8_t row, uint8_t col, bool state) override;
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t ADDR_SIGNAL = 0x0A;
        static constexpr uint8_t ADDR_BATT = 0x08;
        static constexpr uint8_t ADDR_SMALL_SEGS = 0x00;
        static constexpr uint8_t ADDR_LABELS = 0x11;
        static constexpr uint8_t ADDR_BIG_SEGS = 0x0B;

        static constexpr uint8_t RAM_ADDR_LAST = 0x18;
        static constexpr uint8_t RAM_BYTE_COUNT = (RAM_ADDR_LAST + 2) / 2;

        uint8_t _buffer_default[6] = {0};

        static constexpr uint8_t FLAG_COLON_TOP = 0x01;
        static constexpr uint8_t FLAG_COLON_DEFAULT_LEFT = 0x02;
        static constexpr uint8_t FLAG_COLON_DEFAULT_RIGHT = 0x04;

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr int8_t DECIMAL_RAM_OFFSET = -1;  // -1: dot belongs to previous position
        static constexpr uint8_t DECIMAL_TOP_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_TOP_MAX_COL = 2;
        static constexpr uint8_t DECIMAL_TOP_POINT_BIT = 0x08;
        static constexpr uint8_t DECIMAL_BOTTOM_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_BOTTOM_MAX_COL = 4;
        static constexpr uint8_t DECIMAL_BOTTOM_POINT_BIT = 0x10;

        // Colon configuration (separate positions from decimal)
        static constexpr uint8_t COLON_TOP_COL = 2;
        static constexpr uint8_t COLON_BOTTOM_LEFT_COL = 2;
        static constexpr uint8_t COLON_BOTTOM_RIGHT_COL = 4;

        uint8_t _mapSegmentsTop(uint8_t val);
        uint8_t _mapSegments(uint8_t val);
        uint8_t _convertLabelFlags(uint8_t labels);
        size_t _writeRow0(uint8_t ch);
        size_t _writeRow1(uint8_t ch);
};
#endif
