/**
 * @example HT1622/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using HT1621
 */

#include "SegLCD_HT1622_Raw.h"

#define PIN_CS 27
#define PIN_WR 14
#define PIN_DATA 12
#define PIN_PWR -1 // If you have connected Vcc on GPIO instead power line else -1


SegLCD_HT1622_Raw lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  if (PIN_PWR > -1) {
    pinMode(PIN_PWR, OUTPUT);
    digitalWrite(PIN_PWR, HIGH);
  }

  lcd.init();

  // Send RAW command
  lcd.command(CMD_SYS_EN);

  // Send LCD ON implemented command
  lcd.on();

  // Clear LCD
  lcd.clear();

  delay(1000);

  // Write at address 0 one byte
  lcd.writeRam((uint8_t)0xFF, 4);
  lcd.writeRam((uint8_t)0xFF, 6);
}

uint8_t i=0;
void loop() {
  delay(1000);
}
