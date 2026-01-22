#ifndef PCF85176_TEMPHUM_PLUGIN_H
#define PCF85176_TEMPHUM_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_TempHum.h"
#include <Wire.h>

class PCF85176_TempHumPlugin : public LCDPlugin {
public:
    const char *name() const override { return "temphum"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            Wire.begin(cfg.sda, cfg.scl);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_TempHumidity(Wire, cfg.i2cAddr, cfg.subAddr);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        if (cfg.backlight >= 0) {
            lcd->initBacklight(cfg.backlight);
            lcd->setBacklight(true);
        }
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        auto *typed = static_cast<SegLCD_PCF85176_TempHumidity*>(lcd);
        delete typed;
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out; // Unused parameter
        auto *lcd = static_cast<SegLCD_PCF85176_TempHumidity*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "g") == 0) {
            lcd->setSignalLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ls") == 0) {
            lcd->setLabels((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "lc") == 0) {
            lcd->clearLabels((uint8_t)parseNumber(nextToken(&args)));
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
        printMenuLine(out, "temphum commands:");
        printMenuLine(out, "  b <0-4>         - battery level");
        printMenuLine(out, "  g <0-4>         - signal level");
        printMenuLine(out, "  ls/lc <mask>    - set/clear labels");
        printMenuLine(out, "  d <r> <c> <0|1> - decimal");
    }

    bool hasDecimal() const override { return true; }
};

#endif
