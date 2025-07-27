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

  lcd.print("987.654");

  delay(5000);
}

int counter = 0;
void loop() {
  lcd.setBatteryLevel(counter % 4);

  lcd.setCursor(0, 0);
  lcd.print(counter);

  counter++;
  if (counter > 999999) {
    counter = 0;
  }

  delay(1000);
}
