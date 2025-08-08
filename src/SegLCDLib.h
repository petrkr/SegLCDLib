#ifndef SEGLCDLIB_H
#define SEGLCDLIB_H

#include <Arduino.h>
#include "Print.h"

/**
 * @brief Drive mode used by LCD controller.
 */
typedef enum {
    MODE_DRIVE_STATIC = 1, ///< Static drive (1/1 duty)
    MODE_DRIVE_12 = 2,     ///< 1/2 multiplexing
    MODE_DRIVE_13 = 3,     ///< 1/3 multiplexing
    MODE_DRIVE_14 = 0      ///< 1/4 multiplexing
} ModeDrive;

/**
 * @brief Bias mode used by LCD controller. Not care if drive mode is static
 */
typedef enum {
    MODE_BIAS_13 = 0, ///< 1/3 bias
    MODE_BIAS_12 = 1  ///< 1/2 bias
} ModeBias;

/**
 * @brief Abstract base class for LCD segment display drivers.
 *
 * This class defines the generic interface required by all LCD segment drivers,
 * including display control, character output, and data writing methods.
 */
class SegLCDLib : public Print {
    public:
        /**
         * @brief Logical display sections that can be targeted by higher-level rendering logic.
         */

        // ---------------------------
        // LCD API 1.0 Mandatory part
        // ---------------------------
        // Spec: https://playground.arduino.cc/Code/LCDAPI/

        /**
         * @brief Initialize the display driver, clear display and set position to 0,0.
         */
        virtual void init() = 0;

        /**
         * @brief Clear all visible segments on the display.
         */
        virtual void clear();

        /**
         * @brief Set cursor to 0, 0 without clear display
         */
        void home();

        /**
         * @brief Set cursor on exact digit.
         *
         * Because LCD API is mainly used for character displays in this segment displays this function will be littlebit
         * confusing. Because most of Segment LCD has only one row, thus this will be like row 0 and column will represent
         * actual digit.
         *
         * But if there is LCD like T1T2 LCD. Row 0 will be clock part, Row 1 will be T1: part and Row 2 will be T2: part of LCD
         *
         * @param row Row, where 0-MAXROWS
         * @param col Column 0-MAXDIGITS in current row
         */
        virtual void setCursor(uint8_t row, uint8_t col);

        virtual size_t write(uint8_t ch) { };

        /**
         * @brief Send RAW command to controller
         *
         * @param command Raw command byte
         */
        virtual void command(uint8_t command) = 0;
        using Print::write;

        // --------------------------
        // LCD API 1.0 Optional part
        // --------------------------
        /**
         * @brief Turn the display on.
         */
        virtual void on() = 0;

        /**
         * @brief Turn the display off.
         */
        virtual void off() = 0;


        // ----------------------
        // SegLCDLib specific part
        // ----------------------

    protected:
        /**
         * @brief Low-level method to write a single byte to display RAM.
         *
         * @param data Data byte
         * @param address RAM address
         */
        virtual void _writeRam(uint8_t data, uint8_t address) = 0;

        /**
         * @brief Low-level method to write a data buffer to display RAM.
         *
         * @param data Pointer to data buffer
         * @param length Length of data buffer
         * @param address Start RAM address
         */
        virtual void _writeRam(uint8_t *data, size_t length, uint8_t address) = 0;

        /**
         * @brief Helper method to convert a character to its segment representation.
         */
        uint8_t _get_char_value(char ch);

        /**
         * @brief Helper method to convert a character to its 16-segment representation.
         */
        uint16_t _get_16char_value(char ch);

        /**
         * @brief Helper method to count number of digits in an integer.
         */
        int _countDigits(long num);

        /**
         * @brief Current Row
         */
        uint8_t _cursorRow = 0;

        /**
         * @brief Current Column
         */
        uint8_t _cursorCol = 0;
};

#endif
