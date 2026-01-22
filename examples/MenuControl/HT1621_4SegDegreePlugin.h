#ifndef HT1621_4SEGDEGREE_PLUGIN_H
#define HT1621_4SEGDEGREE_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_HT1621_4SegDegree.h"

class HT1621_4SegDegreePlugin : public LCDPlugin {
public:
    const char *name() const override { return "4deg"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        auto *lcd = new SegLCD_HT1621_4SegDegree(cfg.cs, cfg.data, cfg.wr);
        if (cfg.pwr >= 0) {
            lcd->initBacklight(cfg.pwr);
            lcd->setBacklight(true);
        }
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_HT1621_4SegDegree*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_HT1621_4SegDegree*>(lcdBase);

        if (strcmp(cmd, "deg") == 0) {
            lcd->setDegree(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "mdot") == 0) {
            lcd->setMiddleDot(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "colon") == 0) {
            lcd->setColon(0, 1, parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "d") == 0) {
            lcd->setDecimal((uint8_t)parseNumber(nextToken(&args)),
                           (uint8_t)parseNumber(nextToken(&args)),
                           parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "bl") == 0) {
            lcd->setBacklight(parseBool(nextToken(&args)));
            return true;
        }

        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "4deg commands:");
        printMenuLine(out, "  deg <0|1>       - degree symbol");
        printMenuLine(out, "  mdot <0|1>      - middle dot");
        printMenuLine(out, "  colon <0|1>     - colon");
        printMenuLine(out, "  d <r> <c> <0|1> - decimal (c=0-1)");
        printMenuLine(out, "  bl <0|1>        - backlight");
    }

    bool hasDecimal() const override { return true; }
    bool hasColon() const override { return true; }
};

#endif
