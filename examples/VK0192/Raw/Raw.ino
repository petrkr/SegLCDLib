
#include "SegLCD_VK0192_Raw.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21
#define PIN_PWR 15 // If you have connected Vcc on GPIO instead power line else -1

SegTransport3WireArduino transport(PIN_DATA, PIN_WR);
SegLCD_VK0192_Raw raw(transport, PIN_CS);

void setup() {
  if (PIN_PWR > -1) {
    raw.initBacklight(PIN_PWR);
    raw.setBacklight(true);
  }

  raw.init();
  raw.clear();

  raw.writeRam(0x80, 0x24); // Digit 1 (col 0)
  raw.writeRam(0x80, 0x10); // Digit 2 (col 1)
  raw.writeRam(0x80, 0x1E); // Digit 3 (col 2)
  raw.writeRam(0x80, 0x28); // Digit 4 (col 3)
}


void loop() {
  
}
