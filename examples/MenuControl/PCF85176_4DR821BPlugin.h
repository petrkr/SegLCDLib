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
            initI2CBus(cfg);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_4DR821B(Wire, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
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
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "4dr821 commands:");
        printMenuLine(out, "  sym <mask> <0|1>  - symbol");
    }
};

#endif
