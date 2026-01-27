#ifndef PCF85176_ONEDIGIT_PLUGIN_H
#define PCF85176_ONEDIGIT_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_OneDigit.h"
#include <Wire.h>

class PCF85176_OneDigitPlugin : public LCDPlugin {
private:
    bool _reverse = false;
    bool _v1fix = false;

public:
    const char *name() const override { return "1dig"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            Wire.begin(cfg.sda, cfg.scl);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_OneDigit(Wire, cfg.i2cAddr, cfg.subAddr);
        lcd->init(_reverse, _v1fix);
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF85176_OneDigit*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_PCF85176_OneDigit*>(lcdBase);

        if (strcmp(cmd, "rev") == 0) {
            _reverse = parseBool(nextToken(&args));
            lcd->init(_reverse, _v1fix);
            lcd->clear();
            return true;
        }
        if (strcmp(cmd, "v1fix") == 0) {
            _v1fix = parseBool(nextToken(&args));
            lcd->init(_reverse, _v1fix);
            lcd->clear();
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "1dig commands:");
        printMenuLine(out, "  rev <0|1>       - reverse mode");
        printMenuLine(out, "  v1fix <0|1>     - v1 board fix");
    }
};

#endif
