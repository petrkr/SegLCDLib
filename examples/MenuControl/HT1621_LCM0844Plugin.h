#ifndef HT1621_LCM0844_PLUGIN_H
#define HT1621_LCM0844_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_HT1621_LCM0844.h"

class HT1621_LCM0844Plugin : public LCDPlugin {
public:
    const char *name() const override { return "lcm0844"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        auto *lcd = new SegLCD_HT1621_LCM0844(cfg.cs, cfg.data, cfg.wr);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_HT1621_LCM0844*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        auto *lcd = static_cast<SegLCD_HT1621_LCM0844*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "load") == 0) {
            lcd->setLoadLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ls") == 0) {
            lcd->setLabels((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "lc") == 0) {
            lcd->clearLabels((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ss") == 0) {
            lcd->setSymbols((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "sc") == 0) {
            lcd->clearSymbols((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "raw") == 0) {
            const char *addrStr = nextToken(&args);
            const char *valStr = nextToken(&args);
            if (!addrStr || !valStr) {
                out.println("Usage: raw <addr> <value>");
                return true;
            }
            lcd->writeRam((uint8_t)parseNumber(valStr), (uint8_t)parseNumber(addrStr));
            return true;
        }
        if (strcmp(cmd, "rawbuf") == 0) {
            const char *addrStr = nextToken(&args);
            if (!addrStr) {
                out.println("Usage: rawbuf <addr> <v1> <v2> ...");
                return true;
            }

            uint8_t data[64];
            size_t len = 0;
            while (true) {
                const char *valStr = nextToken(&args);
                if (!valStr || len >= sizeof(data)) break;
                data[len++] = (uint8_t)parseNumber(valStr);
            }
            if (len == 0) {
                out.println("Usage: rawbuf <addr> <v1> <v2> ...");
                return true;
            }

            lcd->writeRam(data, len, (uint8_t)parseNumber(addrStr));
            return true;
        }

        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "lcm0844 commands:");
        printMenuLine(out, "  b <0-5>            - battery level");
        printMenuLine(out, "  load <0-5>         - load level");
        printMenuLine(out, "  ls/lc <mask>       - set/clear labels");
        printMenuLine(out, "  ss/sc <mask>       - set/clear symbols");
        printMenuLine(out, "  raw <addr> <v>    - write RAM byte");
        printMenuLine(out, "  rawbuf <a> <v1> <v2> ...");
    }
};

#endif
