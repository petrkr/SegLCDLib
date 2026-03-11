#ifndef SEGLCD_VT0192_5DigSigBattProgress_H
#define SEGLCD_VT0192_5DigSigBattProgress_H

#include <SegDriver_VK0192.h>

/**
 * @file SegLCD_VK0192_5DigSigBattProgress.h
 * @brief VK0192 driver for 5-digit LCD with signal/battery/progress indicators.
 */

/**
 * @brief 5-digit 7-segment LCD with signal/battery/progress (VK0192).
 *
 * Features: signal bars, battery level, progress indicator, unit labels.
 * Integrated VK0192 controller, 3-wire serial protocol.
 * Irregular RAM addressing.
 *
 * @see examples/VK0192/5DigSigBattProgress/5DigSigBattProgress.ino
 */
class SegLCD_VK0192_5DigSigBattProgress : public SegDriver_VK0192 {
    public:
        typedef enum {
            LABEL_P = 0x0001,
            LABEL_T = 0x0002,
            LABEL_PROC = 0x0004,
            LABEL_DEGREE_C = 0x0008,
            LABEL_KPA = 0x0010,
            LABEL_MPA = 0x0020,
            LABEL_M = 0x0040,
            LABEL_A = 0x0080,
            LABEL_V = 0x0100,
        } LabelFlags;

        SegLCD_VK0192_5DigSigBattProgress(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setProgress(uint8_t value);
        void setDegree(bool state);
        void setLabels(uint16_t labels);
        void clearLabels(uint16_t labels);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;

    private:
        void _writeDigit7seg(uint8_t row, uint8_t col, char c);
        void _writeDigit16seg(uint8_t row, uint8_t col, char c);
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t RAM_SIZE = 24;

        // Decimal configuration
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x10;
        static constexpr int8_t DECIMAL_COL_OFFSET = -1;       // Dot belongs to previous digit
        static constexpr int8_t DECIMAL_ADDR_COL_OFFSET = 1;   // Decimal stored in next digit RAM
        static constexpr uint8_t DECIMAL_TOP_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_TOP_MAX_COL = 1;
        static constexpr uint8_t DECIMAL_BOTTOM_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_BOTTOM_MAX_COL = 3;
        static constexpr uint8_t DECIMAL_16SEG_BIT = 0x80;
        static constexpr uint8_t DECIMAL_16SEG_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_16SEG_MAX_COL = 3;
        static constexpr uint8_t DECIMAL_16SEG_ADDR_COL0 = 0x12; // nibble 0x24
        static constexpr uint8_t DECIMAL_16SEG_ADDR_COL1 = 0x08; // nibble 0x10
        static constexpr uint8_t DECIMAL_16SEG_ADDR_COL2 = 0x0F; // nibble 0x1E
        static constexpr uint8_t DECIMAL_16SEG_ADDR_COL3 = 0x14; // nibble 0x28
        static constexpr uint8_t MAX_COL = 4;

        static constexpr uint8_t NUM_7SEG_DIGITS = 10;  // 3+2+5 7-seg digits

        static constexpr uint8_t MAX_SIGNAL_LEVEL = 3;
        static constexpr uint8_t ADDR_SIGNAL_LOW = 0x10;
        static constexpr uint8_t SIGNAL_MASK_LOW = 0xC0;
        static constexpr uint8_t ADDR_SIGNAL_HIGH = 0x16;
        static constexpr uint8_t SIGNAL_MASK_HIGH = 0x80;
        static constexpr uint8_t SIGNAL_LEVEL_SEG[3] = {0x40, 0x80, 0x80};

        static constexpr uint8_t MAX_BATTERY_LEVEL = 4;
        static constexpr uint8_t ADDR_BATT = 0x11;
        static constexpr uint8_t BATTERY_MASK = 0xF0;
        static constexpr uint8_t BATTERY_LEVEL_SEG[4] = {0x10, 0x80, 0x40, 0x20};

        static constexpr uint8_t MAX_PROGRESS = 150;
        static constexpr uint8_t ADDR_PROGRESS_P1 = 0x10;
        static constexpr uint8_t ADDR_PROGRESS_P2 = 0x11;
        static constexpr uint8_t ADDR_PROGRESS_P3 = 0x12;
        static constexpr uint8_t ADDR_PROGRESS_P4 = 0x13;
        static constexpr uint8_t PROGRESS_MASK  = 0x0F;
        static constexpr uint8_t PROGRESS_LEVEL_SEG[16] =
            { 0x01,
              0x08, 0x04, 0x02, 0x01,
              0x01, 0x02, 0x04, 0x08,
              0x08, 0x04, 0x02,
              0x08, 0x04, 0x02, 0x01 };

        static constexpr uint8_t ADDR_LABELS_1 = 0x0E;
        static constexpr uint8_t ADDR_LABELS_2 = 0x0F;
        static constexpr uint8_t ADDR_LABELS_3 = 0x10;

        uint8_t _mapSegments(uint8_t val);
        uint16_t _map16Segments(uint16_t val);
        int8_t _get7SegmentsAddress(uint8_t row, uint8_t col);
        int8_t _get16SegmentsAddress(uint8_t row, uint8_t col);
        void _updateLabels(uint16_t labels, bool set);
};
#endif
