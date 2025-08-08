/**
 * @example HT1622/10Digit16SegmentLCD/10Digit16SegmentLCD.ino
 * @brief Example of 10 digits 16 segment LCD display using HT1622
 */

#include "SegLCD_HT1622_10Dig16Seg.h"

#define PIN_CS 27
#define PIN_WR 14
#define PIN_DATA 12
#define PIN_PWR -1 // If you have connected Vcc on GPIO instead power line else -1


SegLCD_HT1622_10Dig16Seg lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  if (PIN_PWR > -1) {
    pinMode(PIN_PWR, OUTPUT);
    digitalWrite(PIN_PWR, HIGH);
  }

  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  lcd.print("123456789");
  lcd.setDecimal(0, 7, true);
}

uint8_t i=0;
void loop() {
  delay(1000);
}
