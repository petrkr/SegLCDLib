/**
 * @example HT1621/6DigBat/6DigBat.ino
 * @brief Example of 6 digit with battery gauge LCD display using HT1621
 */

#include "SegLCD_HT1621_6SegBat.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21

SegLCD_HT1621_6SegBat lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  lcd.init();
  lcd.clear();

  // Clear LCD
  lcd.print("123456");
  delay(2000);

  lcd.clear();
  lcd.print("1.2.3.4.5.6.");
  delay(2000);

  Serial.println("Testing");
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("12.54");
  delay(1000);

  lcd.setCursor(0, 2);
  lcd.print("23");
  delay(1000);

  lcd.setCursor(0, 4);
  lcd.print("21");
  delay(1000);

  lcd.setCursor(0, 3);
  lcd.print("2.7");
  delay(1000);

  lcd.setCursor(0, 4);
  lcd.print("99");
  delay(1000);


  lcd.setCursor(0, 0);
  lcd.print("123456");
  delay(2000);

  lcd.clear();
}

int counter = 0;
void loop() {
  lcd.setBatteryLevel(counter % 4);

  lcd.setCursor(0, 2);
  lcd.print(float(counter / 10.0));

  counter++;
  if (counter > 999999) {
    counter = 0;
  }

  delay(100);
}
