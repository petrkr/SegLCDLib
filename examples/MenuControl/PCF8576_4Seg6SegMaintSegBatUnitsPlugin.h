#ifndef PCF8576_4SEG6SEGMAINTSEGBATUNITS_PLUGIN_H
#define PCF8576_4SEG6SEGMAINTSEGBATUNITS_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h"
#include "SegTransport.h"
#include <Wire.h>

class PCF8576_4Seg6SegMaintSegBatUnitsPlugin : public LCDPlugin {
public:
    PCF8576_4Seg6SegMaintSegBatUnitsPlugin() : _bus(Wire) {}

    const char *name() const override { return "4s6s"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            initI2CBus(cfg);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF8576_4Seg6SegMaintSegBatUnits(_bus, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF8576_4Seg6SegMaintSegBatUnits*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_PCF8576_4Seg6SegMaintSegBatUnits*>(lcdBase);

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
        if (strcmp(cmd, "deg") == 0) {
            lcd->setDegreeSymbol(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "maint") == 0) {
            lcd->setMaintenanceSymbol(parseBool(nextToken(&args)));
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "4s6s commands:");
        printMenuLine(out, "  b <0-5>            - battery level");
        printMenuLine(out, "  g <0-4>            - signal level");
        printMenuLine(out, "  ls/lc <mask>       - set/clear labels");
        printMenuLine(out, "  deg <0|1>          - degree symbol");
        printMenuLine(out, "  maint <0|1>        - maintenance");
    }

private:
    SegTransportI2CArduino _bus;
};

#endif
