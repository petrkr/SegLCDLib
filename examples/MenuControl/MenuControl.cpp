#include "MenuControl.h"
#include "T1T2Plugin.h"
#include "VK0192Plugin.h"
#include "HT1621_4SegDegreePlugin.h"
#include "HT1621_6SegBatPlugin.h"
#include "HT1621_LCM0844Plugin.h"
#include "HT1621_RawPlugin.h"
#include "HT1622_10Dig16SegPlugin.h"
#include "PCF85176_4DR821BPlugin.h"
#include "PCF85176_2Row4DigSigBatPwrPlugin.h"
#include "PCF85176_6DigSigBattProgressPlugin.h"
#include "PCF85176_OneDigitPlugin.h"
#include "PCF85176_TempHumPlugin.h"
#include "PCx85_RawPlugin.h"
#include "PCF8576_4Seg6SegMaintSegBatUnitsPlugin.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <Preferences.h>
#endif

// Plugin registry
static T1T2Plugin pluginT1T2;
static VK0192Plugin pluginVK0192;
static HT1621_4SegDegreePlugin plugin4deg;
static HT1621_6SegBatPlugin plugin6bat;
static HT1621_LCM0844Plugin pluginLcm0844;
static HT1621_RawPlugin pluginHt1621Raw;
static HT1622_10Dig16SegPlugin plugin16seg;
static PCF85176_4DR821BPlugin plugin4dr821;
static PCF85176_2Row4DigSigBatPwrPlugin plugin2row4dig;
static PCF85176_6DigSigBattProgressPlugin plugin6prog;
static PCF85176_OneDigitPlugin plugin1dig;
static PCF85176_TempHumPlugin pluginTempHum;
static PCx85_RawPlugin pluginPcxRaw;
static PCF8576_4Seg6SegMaintSegBatUnitsPlugin plugin4s6s;

static LCDPlugin *plugins[] = {
    &pluginT1T2, &pluginVK0192,
    &plugin4deg, &plugin6bat, &pluginLcm0844, &pluginHt1621Raw, &plugin16seg,
    &plugin4dr821, &plugin2row4dig, &plugin6prog, &plugin1dig,
    &pluginTempHum, &pluginPcxRaw, &plugin4s6s
};
static const uint8_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

// Global state
static DisplayConfig config;
static SegLCDLib *activeLCD = nullptr;
static LCDPlugin *activePlugin = nullptr;
static bool inSettings = false;

// Configuration persistence
#if defined(ARDUINO_ARCH_ESP32)

static const char* PREFS_NAMESPACE = "seg_lcd";
static const uint8_t PREFS_MAGIC = 0xA5;

bool configLoad(DisplayConfig &cfg) {
    Preferences prefs;
    if (!prefs.begin(PREFS_NAMESPACE, true)) return false;

    if (prefs.getUChar("magic", 0x00) != PREFS_MAGIC) {
        prefs.end();
        return false;
    }

    size_t got = prefs.getBytes("cfg", &cfg, sizeof(cfg));
    prefs.end();
    return got == sizeof(cfg) && cfg.version == 1;
}

bool configSave(const DisplayConfig &cfg) {
    Preferences prefs;
    if (!prefs.begin(PREFS_NAMESPACE, false)) return false;

    prefs.putUChar("magic", PREFS_MAGIC);
    size_t written = prefs.putBytes("cfg", &cfg, sizeof(cfg));
    prefs.end();
    return written == sizeof(cfg);
}

void configClear() {
    Preferences prefs;
    if (!prefs.begin(PREFS_NAMESPACE, false)) return;
    prefs.clear();
    prefs.end();
}

#else
// Non-ESP32 fallback
bool configLoad(DisplayConfig &cfg) { (void)cfg; return false; }
bool configSave(const DisplayConfig &cfg) { (void)cfg; return false; }
void configClear() {}
#endif

// Expose state for debugging
SegLCDLib *mcGetActiveLCD() { return activeLCD; }
LCDPlugin *mcGetActivePlugin() { return activePlugin; }
const DisplayConfig& mcGetConfig() { return config; }

// Status display
static void printStatusLine(const char *text, size_t content_width) {
    size_t len = strlen(text);
    if (len > content_width) {
        len = content_width;
    }
    Serial.print("│ ");
    for (size_t i = 0; i < len; ++i) {
        Serial.print(text[i]);
    }
    for (size_t i = len; i < content_width; ++i) {
        Serial.print(' ');
    }
    Serial.println(" │");
}

static void printStatusBorder(size_t content_width) {
    Serial.print("┌");
    for (size_t i = 0; i < content_width + 2; ++i) {
        Serial.print("─");
    }
    Serial.println("┐");
}

static void printStatusBottom(size_t content_width) {
    Serial.print("└");
    for (size_t i = 0; i < content_width + 2; ++i) {
        Serial.print("─");
    }
    Serial.println("┘");
}

static bool termAnsi = false;

bool mcTermAnsiEnabled() {
    return termAnsi;
}

const char *mcAnsi(const char *code) {
    return termAnsi ? code : "";
}

static void printPrompt() {
    Serial.print(mcAnsi("\x1b[1;32m"));
    Serial.print('[');
    Serial.print(inSettings ? "settings" : "main");
    Serial.print("] > ");
    Serial.print(mcAnsi("\x1b[0m"));
}

static bool detectAnsiTerminal() {
    while (Serial.available()) Serial.read();
    Serial.print("\x1b[6n");

    unsigned long start = millis();
    String resp;
    while (millis() - start < 200) {
        if (Serial.available()) {
            char c = Serial.read();
            resp += c;
            if (c == 'R') break;
        }
    }
    return resp.length() >= 4 && resp[0] == 0x1B && resp[1] == '[' && resp.endsWith("R");
}

static void printStatus() {
    char buf[64];
    char buf2[64];
    size_t width = 0;
    if (activeLCD) {
        snprintf(buf, sizeof(buf), "LCD: %s", activePlugin->name());
        if (config.busType == BUS_I2C) {
            char addr_str[16];
            snprintf(addr_str, sizeof(addr_str), "0x%02X", config.i2cAddr);
            char sub_str[16];
            snprintf(sub_str, sizeof(sub_str), "0x%02X", config.subAddr);
            snprintf(buf2, sizeof(buf2), "I2C: SDA=%d SCL=%d A=%s SA=%s",
                     config.sda, config.scl, addr_str, sub_str);
        } else {
            snprintf(buf2, sizeof(buf2), "3W: CS=%d WR=%d DA=%d",
                     config.cs, config.wr, config.data);
        }
    } else {
        snprintf(buf, sizeof(buf), "LCD: None (use 'settings')");
        buf2[0] = '\0';
    }
    width = strlen(buf);
    if (buf2[0] != '\0') {
        size_t w2 = strlen(buf2);
        if (w2 > width) { width = w2; }
    }
    Serial.println();
    printStatusBorder(width);
    printStatusLine(buf, width);
    if (buf2[0] != '\0') {
        printStatusLine(buf2, width);
    }
    printStatusBottom(width);
}

// Settings help display
static void printSettingsHelp() {
    Serial.println();
    printMenuTop(Serial, "Settings Commands");
    printMenuLine(Serial, "Display Selection:");
    printMenuLine(Serial, "  list          - show available LCDs");
    printMenuLine(Serial, "  select <id>   - choose LCD");
    printMenuLine(Serial, "  bus <type>    - set bus (i2c/3wire)");
    printMenuLine(Serial, "");
    printMenuLine(Serial, "Configuration:");
    printMenuLine(Serial, "  set <p> <v>   - set parameter");
    printMenuLine(Serial, "  get <p>       - get parameter");
    printMenuLine(Serial, "  show          - show full config");
    printMenuLine(Serial, "");
    printMenuLine(Serial, "Storage & Init:");
    printMenuLine(Serial, "  init          - initialize LCD");
    printMenuLine(Serial, "  save          - save config");
    printMenuLine(Serial, "  clear         - clear saved config");
    printMenuLine(Serial, "  pins          - interactive pin setup");
    printMenuLine(Serial, "");
    printMenuLine(Serial, "System:");
    printMenuLine(Serial, "  exit          - return to active mode");
    printMenuLine(Serial, "  help (?, h)   - this menu");
    printMenuBottom(Serial);
}

// Active mode commands
static void handleActiveCommand(char *cmd, char *args) {
    if (!activeLCD && strcmp(cmd, "status") != 0 && strcmp(cmd, "s") != 0 &&
        strcmp(cmd, "settings") != 0 && strcmp(cmd, "help") != 0 &&
        strcmp(cmd, "?") != 0 && strcmp(cmd, "h") != 0 && strcmp(cmd, "q") != 0) {
        Serial.println("No LCD active");
        return;
    }

    // Common commands
    if (strcmp(cmd, "c") == 0) { activeLCD->clear(); return; }
    if (strcmp(cmd, "f") == 0) { activeLCD->flush(); return; }
    if (strcmp(cmd, "dump") == 0) { dumpBuffer(*activeLCD, Serial); return; }
    if (strcmp(cmd, "af") == 0) { activeLCD->setAutoFlush(parseBool(nextToken(&args))); return; }
    if (strcmp(cmd, "bl") == 0) {
        if (config.backlightMode == SegLCDLib::BACKLIGHT_PWM) {
            const char *valStr = nextToken(&args);
            int val = valStr ? (int)parseNumber(valStr) : 0;
            if (val < 0) val = 0;
            if (val > 254) val = 254;
            activeLCD->setBacklight(val);
        } else {
            activeLCD->setBacklight(parseBool(nextToken(&args)));
        }
        return;
    }
    if (strcmp(cmd, "cur") == 0) {
        activeLCD->setCursor((uint8_t)parseNumber(nextToken(&args)),
                            (uint8_t)parseNumber(nextToken(&args)));
        return;
    }
    if (strcmp(cmd, "w") == 0) {
        uint8_t r = (uint8_t)parseNumber(nextToken(&args));
        uint8_t c = (uint8_t)parseNumber(nextToken(&args));
        while (*args == ' ') args++;
        activeLCD->setCursor(r, c);
        activeLCD->print(args);
        return;
    }
    if (strcmp(cmd, "p") == 0) {
        while (*args == ' ') args++;
        activeLCD->print(args);
        return;
    }
    // LCD-specific commands
    if (activePlugin && activePlugin->handleCommand(activeLCD, cmd, args, Serial)) return;

    // Settings/lifecycle/status
    if (strcmp(cmd, "status") == 0 || strcmp(cmd, "s") == 0) {
        printStatus();
        return;
    }
    if (strcmp(cmd, "settings") == 0) {
        inSettings = true;
        printSettingsHelp();
        return;
    }
    if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
        Serial.println("Use 'settings' to reconfigure or reset device.");
        return;
    }
    if (strcmp(cmd, "reset") == 0) {
        Serial.println("Resetting...");
#if defined(ARDUINO_ARCH_ESP32)
        ESP.restart();
#elif defined(ARDUINO_ARCH_AVR)
        asm volatile ("jmp 0");
#else
        // Generic fallback - trigger watchdog or jump to 0
        void (*reset)(void) = 0;
        reset();
#endif
        return;
    }
    if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0 || strcmp(cmd, "h") == 0) {
        Serial.println();
        printMenuTop(Serial, "Commands");
        printMenuLine(Serial, "Display Control:");
        printMenuLine(Serial, "  c             - clear display");
        printMenuLine(Serial, "  p <text>      - print text");
        printMenuLine(Serial, "  w <r> <c> <t> - write at row,col");
        printMenuLine(Serial, "  bl <0|1|0-254> - backlight");
        printMenuLine(Serial, "");
        printMenuLine(Serial, "Cursor & Flush:");
        printMenuLine(Serial, "  cur <r> <c>   - set cursor");
        printMenuLine(Serial, "  f             - flush buffer");
        printMenuLine(Serial, "  af <0|1>      - auto-flush on/off");
        printMenuLine(Serial, "");
        printMenuLine(Serial, "Debug:");
        printMenuLine(Serial, "  dump          - show RAM buffer");
        printMenuLine(Serial, "  status (s)    - display status");
        printMenuLine(Serial, "");
        printMenuLine(Serial, "System:");
        printMenuLine(Serial, "  settings      - config menu");
        printMenuLine(Serial, "  reset         - reboot device");
        printMenuLine(Serial, "  help (?, h)   - this menu");

        if (activePlugin) {
            printMenuLine(Serial, "");
            printMenuSeparator(Serial, "LCD-Specific");
            activePlugin->printMenu(Serial);
        }
        printMenuBottom(Serial);
        return;
    }

    Serial.println("Unknown command.");
}

// Settings mode commands
static void handleSettingsCommand(char *cmd, char *args);

static void printPluginList(Stream &out) {
    out.println("Available displays:");
    for (uint8_t i = 0; i < pluginCount; i++) {
        out.print("  ");
        out.print(i);
        out.print(": ");
        out.println(plugins[i]->name());
    }
}

static const char *backlightModeName(SegLCDLib::BacklightMode mode) {
    return mode == SegLCDLib::BACKLIGHT_PWM ? "pwm" : "digital";
}

static const char *driveModeName(ModeDrive mode) {
    switch (mode) {
        case MODE_DRIVE_STATIC: return "static";
        case MODE_DRIVE_12: return "12";
        case MODE_DRIVE_13: return "13";
        case MODE_DRIVE_14: return "14";
        default: return "?";
    }
}

static const char *biasModeName(ModeBias mode) {
    switch (mode) {
        case MODE_BIAS_12: return "12";
        case MODE_BIAS_13: return "13";
        default: return "?";
    }
}

static bool isRawPlugin(const LCDPlugin *plugin) {
    if (!plugin) return false;
    const char *pluginName = plugin->name();
    return strcmp(pluginName, "pcx85_raw") == 0 || strcmp(pluginName, "ht1621_raw") == 0;
}

static void initBacklightIfConfigured() {
    if (!activeLCD || config.backlight < 0) return;
    activeLCD->initBacklight(config.backlight, config.backlightMode);
}

static void printConfigSummary(Stream &out) {
    out.print("Display ID: ");
    out.print(config.displayId);
    out.println();

    if (config.displayId < pluginCount) {
        out.print("Type: ");
        out.println(plugins[config.displayId]->name());

        if (config.busType == BUS_I2C) {
            out.print("Bus: I2C, SDA=");
            out.print(config.sda);
            out.print(", SCL=");
            out.print(config.scl);
            out.print(", Addr=0x");
            if (config.i2cAddr < 0x10) out.print('0');
            out.print(config.i2cAddr, HEX);
            out.println();
        } else {
            out.print("Bus: 3-WIRE, CS=");
            out.print(config.cs);
            out.print(", WR=");
            out.print(config.wr);
            out.print(", DATA=");
            out.print(config.data);
            out.println();
        }

        if (config.backlight >= 0) {
            out.print("Backlight pin: ");
            out.println(config.backlight);
            out.print("Backlight mode: ");
            out.println(backlightModeName(config.backlightMode));
        }
        if (config.power >= 0) {
            out.print("Power pin: ");
            out.println(config.power);
        }
        if (isRawPlugin(plugins[config.displayId])) {
            out.print("Drive mode: ");
            out.println(driveModeName(config.rawDrive));
            out.print("Bias mode: ");
            out.println(biasModeName(config.rawBias));
        }
    }
}

static void selectDisplay() {
    printPluginList(Serial);
    Serial.print("Select display ID (0-");
    Serial.print(pluginCount - 1);
    Serial.print("): ");

    while (!Serial.available()) delay(10);
    int id = Serial.parseInt();
    if (id >= 0 && id < pluginCount) {
        config.displayId = (uint8_t)id;
        Serial.println("OK");
    } else {
        Serial.println("Invalid");
    }
}

static String readLine() {
    String line;
    while (true) {
        while (!Serial.available()) delay(10);
        char c = Serial.read();
        if (c == 0x04) break; // Ctrl+D
        if (c == '\r' || c == '\n') break;
        if (c == '\b' || c == 0x7F) {
            if (line.length() > 0) {
                Serial.print("\b \b");
                line.remove(line.length() - 1);
            }
            continue;
        }
        if (c >= 32 && c <= 126) Serial.print(c);
        line += c;
    }
    Serial.println();
    while (Serial.available()) {
        char c = Serial.peek();
        if (c != '\r' && c != '\n') break;
        Serial.read();
    }
    return line;
}

static void configurePins() {
    if (config.displayId >= pluginCount) {
        Serial.println("Select display first");
        return;
    }

    if (config.busType == BUS_I2C) {
        Serial.print("SDA pin (-1=default) [-1]: ");
        String sdaStr = readLine();
        config.sda = sdaStr.length() ? (int8_t)parseNumber(sdaStr.c_str()) : -1;
        Serial.println(config.sda);

        Serial.print("SCL pin (-1=default) [-1]: ");
        String sclStr = readLine();
        config.scl = sclStr.length() ? (int8_t)parseNumber(sclStr.c_str()) : -1;
        Serial.println(config.scl);

        Serial.print("I2C Address (hex, e.g. 38) [38]: 0x");
        String addrStr = readLine();
        if (addrStr.length()) {
            config.i2cAddr = (uint8_t)strtol(addrStr.c_str(), nullptr, 16);
        }
        Serial.println(config.i2cAddr, HEX);

        Serial.print("I2C Sub-Address (hex, e.g. 00) [00]: 0x");
        String subAddrStr = readLine();
        if (subAddrStr.length()) {
            config.subAddr = (uint8_t)strtol(subAddrStr.c_str(), nullptr, 16);
        }
        Serial.println(config.subAddr, HEX);
    } else {
        Serial.print("CS pin (-1=skip): ");
        config.cs = (int8_t)parseNumber(readLine().c_str());
        Serial.println(config.cs);

        Serial.print("WR pin (-1=skip): ");
        config.wr = (int8_t)parseNumber(readLine().c_str());
        Serial.println(config.wr);

        Serial.print("DATA pin (-1=skip): ");
        config.data = (int8_t)parseNumber(readLine().c_str());
        Serial.println(config.data);
    }

    Serial.print("Backlight pin (-1=none) [-1]: ");
    String backlightStr = readLine();
    config.backlight = backlightStr.length() ? (int8_t)parseNumber(backlightStr.c_str()) : -1;
    Serial.println(config.backlight);
    if (config.backlight >= 0) {
        Serial.print("Backlight mode (digital/pwm) [digital]: ");
        String modeStr = readLine();
        modeStr.toLowerCase();
        config.backlightMode = (modeStr == "pwm") ? SegLCDLib::BACKLIGHT_PWM
                                                  : SegLCDLib::BACKLIGHT_DIGITAL;
        Serial.println(backlightModeName(config.backlightMode));
    }

    Serial.print("Power pin (-1=none) [-1]: ");
    String powerStr = readLine();
    config.power = powerStr.length() ? (int8_t)parseNumber(powerStr.c_str()) : -1;
    Serial.println(config.power);

}

static void handleSettingsCommand(char *cmd, char *args) {
    if (strcmp(cmd, "list") == 0) { printPluginList(Serial); return; }
    if (strcmp(cmd, "show") == 0) { printConfigSummary(Serial); return; }
    if (strcmp(cmd, "select") == 0) {
        char *idStr = nextToken(&args);
        if (!idStr) {
            printPluginList(Serial);
            return;
        }
        int id = (int)parseNumber(idStr);
        if (id >= 0 && id < pluginCount) {
            config.displayId = (uint8_t)id;
            Serial.print("OK: ");
            Serial.println(plugins[id]->name());
        } else {
            Serial.println("Invalid display ID");
        }
        return;
    }
    if (strcmp(cmd, "bus") == 0) {
        char *busStr = nextToken(&args);
        if (!busStr) {
            Serial.println("Current bus: ");
            Serial.println(config.busType == BUS_I2C ? "I2C" : "3-WIRE");
            return;
        }
        if (strcmp(busStr, "i2c") == 0) {
            config.busType = BUS_I2C;
            Serial.println("OK: Bus = I2C");
        } else if (strcmp(busStr, "3wire") == 0) {
            config.busType = BUS_3WIRE;
            Serial.println("OK: Bus = 3-WIRE");
        } else {
            Serial.println("Invalid bus type (use: i2c or 3wire)");
        }
        return;
    }
    if (strcmp(cmd, "set") == 0) {
        char *param = nextToken(&args);
        char *value = nextToken(&args);
        if (!param || !value) {
            Serial.println("Usage: set <param> <value>");
            Serial.println("Params: sda, scl, addr, cs, wr, data, backlight, blmode, power, drive, bias");
            return;
        }

        if (strcmp(param, "sda") == 0) {
            config.sda = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "scl") == 0) {
            config.scl = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "addr") == 0) {
            config.i2cAddr = (uint8_t)strtol(value, nullptr, 16);
            Serial.println("OK");
        } else if (strcmp(param, "cs") == 0) {
            config.cs = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "wr") == 0) {
            config.wr = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "data") == 0) {
            config.data = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "backlight") == 0) {
            config.backlight = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "power") == 0) {
            config.power = (int8_t)parseNumber(value);
            Serial.println("OK");
        } else if (strcmp(param, "blmode") == 0) {
            if (strcmp(value, "pwm") == 0) {
                config.backlightMode = SegLCDLib::BACKLIGHT_PWM;
                Serial.println("OK");
            } else if (strcmp(value, "digital") == 0) {
                config.backlightMode = SegLCDLib::BACKLIGHT_DIGITAL;
                Serial.println("OK");
            } else {
                Serial.println("Invalid backlight mode (digital/pwm)");
            }
        } else if (strcmp(param, "drive") == 0) {
            ModeDrive drive;
            if (!parseDriveMode(value, drive)) {
                Serial.println("Invalid drive (static|12|13|14)");
            } else {
                config.rawDrive = drive;
                Serial.println("OK");
            }
        } else if (strcmp(param, "bias") == 0) {
            ModeBias bias;
            if (!parseBiasMode(value, bias)) {
                Serial.println("Invalid bias (12|13)");
            } else {
                config.rawBias = bias;
                Serial.println("OK");
            }
        } else {
            Serial.println("Unknown parameter");
        }
        return;
    }
    if (strcmp(cmd, "get") == 0) {
        char *param = nextToken(&args);
        if (!param) {
            printConfigSummary(Serial);
            return;
        }

        if (strcmp(param, "sda") == 0) {
            Serial.println(config.sda);
        } else if (strcmp(param, "scl") == 0) {
            Serial.println(config.scl);
        } else if (strcmp(param, "addr") == 0) {
            Serial.print("0x");
            if (config.i2cAddr < 0x10) Serial.print('0');
            Serial.println(config.i2cAddr, HEX);
        } else if (strcmp(param, "cs") == 0) {
            Serial.println(config.cs);
        } else if (strcmp(param, "wr") == 0) {
            Serial.println(config.wr);
        } else if (strcmp(param, "data") == 0) {
            Serial.println(config.data);
        } else if (strcmp(param, "backlight") == 0) {
            Serial.println(config.backlight);
        } else if (strcmp(param, "power") == 0) {
            Serial.println(config.power);
        } else if (strcmp(param, "blmode") == 0) {
            Serial.println(backlightModeName(config.backlightMode));
        } else if (strcmp(param, "drive") == 0) {
            Serial.println(driveModeName(config.rawDrive));
        } else if (strcmp(param, "bias") == 0) {
            Serial.println(biasModeName(config.rawBias));
        } else {
            Serial.println("Unknown parameter");
        }
        return;
    }
    if (strcmp(cmd, "pins") == 0) { configurePins(); return; }
    if (strcmp(cmd, "save") == 0) {
        if (configSave(config)) {
            Serial.println("Config saved");
        } else {
            Serial.println("Save failed");
        }
        return;
    }
    if (strcmp(cmd, "clear") == 0) {
        configClear();
        Serial.println("Config cleared");
        return;
    }
    if (strcmp(cmd, "init") == 0) {
        if (config.displayId >= pluginCount) {
            Serial.println("Invalid display ID");
            return;
        }
        if (activeLCD) {
            activePlugin->destroy(activeLCD);
            activeLCD = nullptr;
            activePlugin = nullptr;
        }
        activePlugin = plugins[config.displayId];
        activeLCD = activePlugin->create(config);
        if (activeLCD) {
            initBacklightIfConfigured();
            Serial.println("LCD initialized");
            inSettings = false;
        } else {
            Serial.println("LCD init failed");
        }
        return;
    }
    if (strcmp(cmd, "exit") == 0) {
        if (activeLCD) {
            inSettings = false;
        } else {
            Serial.println("No LCD active");
        }
        return;
    }
    if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0 || strcmp(cmd, "h") == 0) {
        printSettingsHelp();
        return;
    }

    Serial.println("Unknown command");
}

// Serial line parsing and dispatch
static void processLine(String &line) {
    if (line.length() == 0) return;

    // Copy to modifiable buffer
    char buf[256];
    line.toCharArray(buf, sizeof(buf));

    // Parse command and args
    char *cmd = buf;
    while (*cmd == ' ') cmd++;
    if (*cmd == '\0') return;

    char *args = cmd;
    while (*args && *args != ' ') args++;
    if (*args) {
        *args = '\0';
        args++;
    }

    // Dispatch
    if (inSettings) {
        handleSettingsCommand(cmd, args);
    } else {
        handleActiveCommand(cmd, args);
    }
}

// Setup and loop
void mcSetup() {
    Serial.begin(115200);
    delay(2000);

    Serial.print("terminal detect: ");
    termAnsi = detectAnsiTerminal();
    if (termAnsi) {
        Serial.print(mcAnsi("\x1b[1;32m"));
        Serial.println("ANSI");
        Serial.print(mcAnsi("\x1b[0m"));
    } else {
        Serial.println("PLAIN");
    }
    // Banner
    Serial.println();
    Serial.print(mcAnsi("\x1b[1;36m"));
    Serial.println("╔══════════════════════════════════════╗");
    Serial.println("║      SegLCDLib MenuControl v0.0      ║");
    Serial.println("╚══════════════════════════════════════╝");
    Serial.print(mcAnsi("\x1b[0m"));
    Serial.println();

    // Try to load saved config
    if (!configLoad(config)) {
        Serial.println("No saved config. Enter settings mode.");
        inSettings = true;
    } else {
        // Try to init LCD with saved config
        if (config.displayId < pluginCount) {
            activePlugin = plugins[config.displayId];
            activeLCD = activePlugin->create(config);
            if (activeLCD) {
                initBacklightIfConfigured();
                Serial.println("LCD initialized from saved config");
                inSettings = false;
            } else {
                Serial.println("Failed to initialize LCD. Entering settings.");
                inSettings = true;
            }
        } else {
            Serial.println("Invalid saved config. Entering settings.");
            inSettings = true;
        }
    }

    printStatus();

    if (inSettings) {
        Serial.println("Type 'help' for settings commands");
    } else {
        Serial.println("Type 'help' for active commands");
    }
    printPrompt();
}

void mcLoop() {
    if (!Serial.available()) return;

    static String line;
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
        Serial.println();  // Echo newline
        processLine(line);
        line = "";
        printPrompt();
    } else if (c == 0x04) {
        if (inSettings) inSettings = false;
        line = "";
        Serial.println();
        printPrompt();
    } else if (c == '\b' || c == 0x7F) {
        // Backspace
        if (line.length() > 0) {
            Serial.print("\b \b");
            line.remove(line.length() - 1);
        }
    } else if (c >= 32 && c <= 126) {
        // Printable
        Serial.print(c);
        line += c;
    }
}
