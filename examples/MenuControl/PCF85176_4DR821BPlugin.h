#ifndef PCF85176_4DR821B_PLUGIN_H
#define PCF85176_4DR821B_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_4DR821B.h"
#include <Wire.h>

class PCF85176_4DR821BPlugin : public LCDPlugin {
public:
    const char *name() const override { return "4dr821"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            Wire.begin(cfg.sda, cfg.scl);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_4DR821B(Wire, cfg.i2cAddr, cfg.subAddr);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF85176_4DR821B*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_PCF85176_4DR821B*>(lcdBase);

        if (strcmp(cmd, "sym") == 0) {
            lcd->setSymbol((uint8_t)parseNumber(nextToken(&args)), parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "colon") == 0) {
            uint8_t pos = (uint8_t)parseNumber(nextToken(&args));
            lcd->setColon(0, pos, parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "d") == 0) {
            lcd->setDecimal((uint8_t)parseNumber(nextToken(&args)),
                           (uint8_t)parseNumber(nextToken(&args)),
                           parseBool(nextToken(&args)));
            return true;
        }

        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "4dr821 commands:");
        printMenuLine(out, "  sym <mask> <0|1>  - symbol");
        printMenuLine(out, "  colon <pos> <0|1> - colon (0/1)");
        printMenuLine(out, "  d <r> <c> <0|1>   - decimal (c=0-2)");
    }

    bool hasDecimal() const override { return true; }
    bool hasColon() const override { return true; }
};

#endif
