/**
 * @example 4DigDeg.ino
 * @brief Example of 4 digits with degree symbol LCD display using HT1621
 */

#include "SegLCD_HT1621_4SegDegree.h"

#define PIN_CS 26
#define PIN_WR 27
#define PIN_DATA 14
#define PIN_PWR 12 // If you have connected Vcc on GPIO instead power line else -1

SegLCD_HT1621_4SegDegree lcd(PIN_CS, PIN_DATA, PIN_WR);

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
  lcd.print("1.2:34");

  delay(1000);

  lcd.home();
  lcd.print("5678");
  delay(1000);
}

void loop() {
  lcd.home();
  lcd.setDegree(true);
  lcd.print("3.17C");
  delay(5000);

  lcd.home();
  lcd.setDegree(false);
  lcd.print("00:56");
  delay(5000);
}
