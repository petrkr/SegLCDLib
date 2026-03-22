#ifndef PCX85_RAW_PLUGIN_H
#define PCX85_RAW_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCx85_Raw.h"
#include <Wire.h>

class PCx85_RawPlugin : public LCDPlugin {
private:
    ModeDrive _drive = MODE_DRIVE_14;
    ModeBias _bias = MODE_BIAS_13;

public:
    const char *name() const override { return "pcx85_raw"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            initI2CBus(cfg);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCx85_Raw(Wire, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
        _drive = cfg.rawDrive;
        _bias = cfg.rawBias;
        lcd->init(_drive, _bias);
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCx85_Raw*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        auto *lcd = static_cast<SegLCD_PCx85_Raw*>(lcdBase);

        if (strcmp(cmd, "raw") == 0) {
            const char *addrStr = nextToken(&args);
            const char *valStr = nextToken(&args);
            if (!addrStr || !valStr) {
                out.println("Usage: raw <addr> <value>");
                return true;
            }
            uint8_t addr = (uint8_t)parseNumber(addrStr);
            uint8_t val = (uint8_t)parseNumber(valStr);
            lcd->writeRam(val, addr);
            return true;
        }
        if (strcmp(cmd, "rawbuf") == 0) {
            const char *addrStr = nextToken(&args);
            if (!addrStr) {
                out.println("Usage: rawbuf <addr> <v1> <v2> ...");
                return true;
            }
            uint8_t addr = (uint8_t)parseNumber(addrStr);
            uint8_t data[64];
            size_t len = 0;
            while (true) {
                const char *v = nextToken(&args);
                if (!v) break;
                if (len >= sizeof(data)) break;
                data[len++] = (uint8_t)parseNumber(v);
            }
            if (len == 0) {
                out.println("Usage: rawbuf <addr> <v1> <v2> ...");
                return true;
            }
            lcd->writeRam(data, len, addr);
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "pcx85_raw commands:");
        printMenuLine(out, "  raw <addr> <value>   - write RAM byte");
        printMenuLine(out, "  rawbuf <addr> <v1> <v2> ...");
    }
};

#endif
