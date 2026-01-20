/**
 * @example HT1622/10Digit16SegmentLCD/10Digit16SegmentLCD.ino
 * @brief Example of 10 digits 16 segment LCD display using HT1622
 */

#include "SegLCD_HT1622_10Dig16Seg.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21
#define PIN_PWR 7 // If you have connected Vcc on GPIO instead power line else -1


SegLCD_HT1622_10Dig16Seg lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  if (PIN_PWR > -1) {
    lcd.initBacklight(PIN_PWR);
    lcd.setBacklight(true);
  }

  lcd.init();

  Serial.println("Testing clock colon");
  lcd.clear();
  lcd.print("12.54");
  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("23");
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("45.6");
  delay(1000);

  // Clear LCD
  lcd.clear();

  delay(1000);

  lcd.print("12.34.56789");

  delay(1000);
  lcd.home();
  lcd.print("99");
}

uint8_t i=0;
void loop() {
  delay(1000);
}
