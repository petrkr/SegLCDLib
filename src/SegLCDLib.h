#ifndef SEGLCDLIB_H
#define SEGLCDLIB_H

#include <Arduino.h>

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
class SegLCDLib {
    public:
        /**
         * @brief Logical display sections that can be targeted by higher-level rendering logic.
         */
        enum class LCDSections {
            SECTION_DEFAULT,  ///< Default section (fallback)
            SECTION_TOP,      ///< Top display line or part
            SECTION_BOTTOM,   ///< Bottom display line or part
            SECTION_T1,       ///< Custom section T1
            SECTION_T2,       ///< Custom section T2
            SECTION_CLOCK,    ///< Clock-related segment section
            SECTION_DAY,      ///< Day indicator section
            SECTION_TEMP,     ///< Temperature section
            SECTION_HUMIDITY  ///< Humidity section
        };

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
        virtual void clear() = 0;

        /**
         * @brief Set cursor to 0, 0 without clear display
         */
        virtual void home() { };

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
        virtual void setCursor(uint8_t row, uint8_t col) { };

        /**
         * @brief Send RAW command to controller
         *
         * @param command Raw command byte
         */
        virtual void command(uint8_t command) = 0;

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

        /**
         * @brief Write a character to a specific digit/position in a defined section.
         *
         * Default implementation does nothing; must be overridden by subclass if needed.
         *
         * @param digit Index of the digit (position on display)
         * @param c Character to be rendered
         * @param section Display section target
         */
        virtual void writeChar(uint8_t digit, char c, LCDSections section) { };

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
         * @brief Helper method to count number of digits in an integer.
         */
        int _countDigits(long num);
};

#endif
