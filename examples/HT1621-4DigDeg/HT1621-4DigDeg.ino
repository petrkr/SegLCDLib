/**
 * @example HT1621-4DigDeg.ino
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

  lcd.begin();

  // Clear LCD
  lcd.clear();

  lcd.setDecimal(2, true);
}

void loop() {
  lcd.setMiddleDot(false);
  lcd.setDegree(true);

  lcd.writeChar(1, '3');
  lcd.writeChar(2, '1');
  lcd.writeChar(3, '7');
  lcd.writeChar(4, 'C');

  delay(5000);

  lcd.setMiddleDot(true);
  lcd.setDegree(false);

  lcd.writeChar(1, '1');
  lcd.writeChar(2, '3');
  lcd.writeChar(3, '5');
  lcd.writeChar(4, '6');

  delay(5000);
}
