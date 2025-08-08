/**
 * @example PCF85176/OneDigit/OneDigit.ino
 * @brief Example of Single digit LCD with 5 digits
 */

#include "SegLCD_PCF85176_OneDigit.h"

#include <Wire.h>

SegLCD_PCF85176_OneDigit lcd(Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin(0, 1);

  Serial.println("Initialize LCD...");
  lcd.init(true, true); // Example display has reversed digits and it  is version 1, so it need fix

  lcd.print("H.ELLO.");
  delay(2000);

  lcd.clear();
  lcd.print(1.2);
  delay(2000);

  lcd.home();
  lcd.print(3.14);
  delay(2000);
}


void loop() {
  lcd.clear();
  lcd.print(1);
  delay(1000);

  lcd.clear();
  lcd.print(12);
  delay(1000);

  lcd.clear();
  lcd.print(123);
  delay(1000);

  lcd.clear();
  lcd.print((float)123.45);
  delay(2000);

  lcd.clear();
  lcd.print((float)3.14);
  delay(2000);

  lcd.clear();
  lcd.print('1');
  lcd.print('2');
  lcd.print('3');
  lcd.print('4');
  lcd.print('5');
  delay(2000);

  lcd.clear();
  delay(500);

  lcd.print("2   5.");
  delay(2000);

  lcd.setDecimal(0, 4, false);
  lcd.setDecimal(0, 0, true);
  delay(2000);

  lcd.clear();
  delay(2000);


  lcd.clear();
  for (int i = 0; i < 1001; i++) {
    lcd.home();
    lcd.print((float)i/10);
    delay(100);
  }

  delay(1000);
}
