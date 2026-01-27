#ifndef LCD_PLUGIN_H
#define LCD_PLUGIN_H

#include <Arduino.h>
#include <ctype.h>
#include "SegLCDLib.h"

// Configuration structure - same as UniversalMenuControl
enum BusType : uint8_t { BUS_I2C = 0, BUS_3WIRE = 1 };

struct DisplayConfig {
    uint8_t version = 1;
    uint8_t displayId = 0;
    uint8_t busType = BUS_I2C;
    int8_t sda = -1, scl = -1;
    int8_t cs = -1, wr = -1, data = -1;
    int8_t backlight = -1;
    SegLCDLib::BacklightMode backlightMode = SegLCDLib::BACKLIGHT_DIGITAL;
    uint8_t i2cAddr = 0x38, subAddr = 0x00;
};

// Terminal ANSI helpers (implemented in MenuControl.cpp)
bool mcTermAnsiEnabled();
const char *mcAnsi(const char *code);

// Helper functions
static long parseNumber(const char *s) {
    if (!s) return 0;
    return strtol(s, nullptr, 0);
}

static const size_t MENU_INNER_WIDTH = 38;

static inline void printMenuLine(Stream &out, const char *text) {
    size_t len = strlen(text);
    if (len > MENU_INNER_WIDTH) len = MENU_INNER_WIDTH;
    const char *c_reset = mcAnsi("\x1b[0m");
    const char *c_border = mcAnsi("\x1b[1;36m");
    const char *c_heading = mcAnsi("\x1b[1;36m");
    const char *c_cmd = mcAnsi("\x1b[1;33m");
    const char *c_param = mcAnsi("\x1b[35m");

    out.print(c_border);
    out.print("│ ");
    out.print(c_reset);

    bool header = (len > 0 && text[len - 1] == ':' && (len < 2 || text[0] != ' '));
    if (header) out.print(c_heading);

    size_t cmd_start = SIZE_MAX;
    size_t cmd_end = SIZE_MAX;
    if (len >= 2 && text[0] == ' ' && text[1] == ' ') {
        size_t s = 2;
        while (s < len && text[s] == ' ') s++;
        size_t e = s;
        while (e < len && text[e] != ' ') e++;
        if (s < len) { cmd_start = s; cmd_end = e; }
    }

    bool in_param = false;
    for (size_t i = 0; i < len; ++i) {
        char ch = text[i];
        bool in_cmd = (cmd_start != SIZE_MAX && i >= cmd_start && i < cmd_end);

        if (!header) {
            if (ch == '<') { out.print(c_param); in_param = true; }
            if (!in_param && cmd_start != SIZE_MAX && i == cmd_start) {
                out.print(c_cmd);
            }
        }

        out.print(ch);

        if (!header) {
            if (!in_param && cmd_end != SIZE_MAX && i + 1 == cmd_end) out.print(c_reset);
            if (ch == '>') { in_param = false; out.print(c_reset); }
        }
    }
    if (header) out.print(c_reset);
    for (size_t i = len; i < MENU_INNER_WIDTH; ++i) out.print(' ');
    out.print(c_border);
    out.println(" │");
    out.print(c_reset);
}

static inline void printMenuTop(Stream &out, const char *title) {
    const char *c_border = mcAnsi("\x1b[1;36m");
    const char *c_reset = mcAnsi("\x1b[0m");
    const char *c_heading = mcAnsi("\x1b[1;36m");
    size_t title_len = strlen(title);
    size_t fixed = title_len + 3; // "─ " + title + " "
    size_t dashes = (MENU_INNER_WIDTH + 2 > fixed) ? (MENU_INNER_WIDTH + 2 - fixed) : 0;
    out.print(c_border);
    out.print("┌─ ");
    out.print(c_heading);
    out.print(title);
    out.print(c_border);
    out.print(' ');
    for (size_t i = 0; i < dashes; ++i) out.print("─");
    out.print("┐");
    out.println(c_reset);
}

static inline void printMenuSeparator(Stream &out, const char *title) {
    const char *c_border = mcAnsi("\x1b[1;36m");
    const char *c_reset = mcAnsi("\x1b[0m");
    const char *c_heading = mcAnsi("\x1b[1;36m");
    size_t title_len = strlen(title);
    size_t fixed = title_len + 3; // "─ " + title + " "
    size_t dashes = (MENU_INNER_WIDTH + 2 > fixed) ? (MENU_INNER_WIDTH + 2 - fixed) : 0;
    out.print(c_border);
    out.print("├─ ");
    out.print(c_heading);
    out.print(title);
    out.print(c_border);
    out.print(' ');
    for (size_t i = 0; i < dashes; ++i) out.print("─");
    out.print("┤");
    out.println(c_reset);
}

static inline void printMenuBottom(Stream &out) {
    const char *c_border = mcAnsi("\x1b[1;36m");
    const char *c_reset = mcAnsi("\x1b[0m");
    out.print(c_border);
    out.print("└");
    for (size_t i = 0; i < MENU_INNER_WIDTH + 2; ++i) out.print("─");
    out.print("┘");
    out.println(c_reset);
}

static bool parseBool(const char *s) {
    if (!s) return false;
    if (strcmp(s, "1") == 0) return true;
    if (strcasecmp(s, "on") == 0) return true;
    if (strcasecmp(s, "true") == 0) return true;
    return false;
}

static char *nextToken(char **args) {
    if (!args || !*args) return nullptr;
    char *s = *args;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '\0') { *args = s; return nullptr; }
    char *tok = s;
    while (*s && *s != ' ' && *s != '\t') s++;
    if (*s) { *s = '\0'; s++; }
    *args = s;
    return tok;
}

static void dumpBuffer(const SegLCDLib &lcd, Stream &out) {
    const uint8_t *ram = lcd.getRamBuffer();
    size_t ramSize = lcd.getRamBufferSize();
    if (!ram || ramSize == 0) {
        out.println("No buffer available.");
        return;
    }

    const char *c_reset = mcAnsi("\x1b[0m");
    const char *c_hdr = mcAnsi("\x1b[1;36m");
    const char *c_one = mcAnsi("\x1b[1;32m");
    const char *c_zero = mcAnsi("\x1b[2;37m");

    // Hexdump
    out.println("HEX:");
    for (size_t i = 0; i < ramSize; i += 16) {
        char off[5];
        snprintf(off, sizeof(off), "%04X", (unsigned)i);
        out.print(c_hdr); out.print(off); out.print(c_reset); out.print(": ");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < ramSize) {
                uint8_t v = ram[i + j];
                if (v < 0x10) out.print('0');
                out.print(v, HEX); out.print(' ');
                if (j == 7) out.print(' ');
            } else {
                out.print("   ");
                if (j == 7) out.print(' ');
            }
        }
        out.print(" |");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < ramSize) {
                uint8_t v = ram[i + j];
                out.print((v >= 32 && v <= 126) ? (char)v : '.');
            } else out.print(' ');
        }
        out.println('|');
    }

    // COM bits
    out.println("\nCOM:");
    const uint8_t perRow = 16;
    const size_t addrSize = ramSize * 2;
    for (size_t base = 0; base < addrSize; base += perRow) {
        out.print(c_hdr);
        out.print("addr:");
        out.print(c_reset);
        for (size_t addr = base; addr < base + perRow && addr < addrSize; addr++) {
            out.print(' ');
            out.print(c_hdr);
            if (addr < 0x10) out.print('0');
            out.print((uint16_t)addr, HEX);
            out.print(c_reset);
            if (((addr - base + 1) % 4) == 0) out.print(' ');
        }
        out.println();
        for (int8_t b = 0; b <= 3; b++) {
            out.print(c_hdr);
            out.print('C'); out.print((char)('0' + b)); out.print("  :");
            out.print(c_reset);
            for (size_t addr = base; addr < base + perRow && addr < addrSize; addr++) {
                size_t i = addr / 2;
                uint8_t bitBase = (addr & 0x01) ? 0 : 4;
                uint8_t bit = bitBase + (3 - b);
                uint8_t v = (ram[i] >> bit) & 0x01;
                out.print(' ');
                out.print(v ? c_one : c_zero);
                out.print(v);
                out.print(c_reset);
                out.print(' ');
                if (((addr - base + 1) % 4) == 0) out.print(' ');
            }
            out.println();
        }
        out.println();
    }
}

// Plugin interface
class LCDPlugin {
public:
    virtual const char *name() const = 0;

    // Create LCD instance - returns nullptr on error
    virtual SegLCDLib* create(const DisplayConfig &cfg) = 0;
    virtual void destroy(SegLCDLib *lcd) = 0;

    // Handle LCD-specific commands
    // Returns true if command was handled, false otherwise
    virtual bool handleCommand(SegLCDLib *lcd, const char *cmd, char *args, Stream &out) = 0;
    virtual void printMenu(Stream &out) = 0;

    virtual ~LCDPlugin() {}
};

#endif
