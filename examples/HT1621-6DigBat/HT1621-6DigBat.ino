/**
 * @example HT1621-6DigBat.ino
 * @brief Example of 6 digit with battery gauge LCD display using HT1621
 */

#include "SegLCD_HT1621_6SegBat.h"

#define PIN_CS 6
#define PIN_WR 4
#define PIN_DATA 7

SegLCD_HT1621_6SegBat lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  lcd.writeChar(1, '9');
  lcd.writeChar(2, '8');
  lcd.writeChar(3, '7');
  lcd.writeChar(4, '6');
  lcd.writeChar(5, '5');
  lcd.writeChar(6, '4');

//  lcd.setDecimal(3, true);
//  lcd.setDecimal(4, true);
//  lcd.setDecimal(5, true);

  delay(5000);
}

uint8_t i=1;
void loop() {
  lcd.setBatteryLevel(i);

  lcd.writeChar(i, i + '0');

  i++;

  if (i>6) {
    i = 1;
  }

  delay(1000);
}
