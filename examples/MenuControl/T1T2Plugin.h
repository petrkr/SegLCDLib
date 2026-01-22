#ifndef T1T2_PLUGIN_H
#define T1T2_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_T1T2Lcd.h"
#include <Wire.h>

class T1T2Plugin : public LCDPlugin {
public:
    const char *name() const override { return "T1T2"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            Wire.begin(cfg.sda, cfg.scl);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_T1T2Lcd(Wire, cfg.i2cAddr, cfg.subAddr);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF85176_T1T2Lcd*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        auto *lcd = static_cast<SegLCD_PCF85176_T1T2Lcd*>(lcdBase);

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
        if (strcmp(cmd, "t1t2s") == 0) {
            lcd->setT1T2Labels((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "t1t2c") == 0) {
            lcd->clearT1T2Labels((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "clk") == 0) {
            lcd->setClockSymbol(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "colon") == 0) {
            lcd->setColon(0, 2, parseBool(nextToken(&args)));
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
        printMenuLine(out, "T1T2 commands:");
        printMenuLine(out, "  b <0-4>         - battery level");
        printMenuLine(out, "  g <0-5>         - signal level");
        printMenuLine(out, "  ls/lc <mask>    - set/clear labels");
        printMenuLine(out, "  t1t2s/c <mask>  - set/clear T1/T2");
        printMenuLine(out, "  clk <0|1>       - clock symbol");
        printMenuLine(out, "  colon <0|1>     - colon symbol");
        printMenuLine(out, "  d <r> <c> <0|1> - decimal point");
    }

    // Capabilities
    bool hasDecimal() const override { return true; }
    bool hasColon() const override { return true; }
};

#endif
