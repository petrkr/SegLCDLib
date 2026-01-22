/*
 * SegLCDLib MenuControl
 *
 * Pluginable menu control system for SegLCDLib displays.
 * Supports multiple LCD types (T1T2, VK0192, etc.) with
 * settings persistence on ESP32.
 *
 * Commands:
 *   help     - show command list
 *   settings - enter settings mode
 *   c        - clear display
 *   p <text> - print text
 *   w <r> <c> <text> - write at position
 *
 * Display-specific commands available per LCD type.
 */

#include "MenuControl.h"

void setup() {
    mcSetup();
}

void loop() {
    mcLoop();
}
