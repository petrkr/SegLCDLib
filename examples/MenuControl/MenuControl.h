#ifndef MENU_CONTROL_H
#define MENU_CONTROL_H

#include "LCDPlugin.h"

// Configuration persistence
bool configLoad(DisplayConfig &cfg);
bool configSave(const DisplayConfig &cfg);
void configClear();

// Menu control setup/loop
void mcSetup();
void mcLoop();

// Internal state (for debugging)
extern SegLCDLib *mcGetActiveLCD();
extern LCDPlugin *mcGetActivePlugin();
extern const DisplayConfig& mcGetConfig();

#endif
