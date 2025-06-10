/**
 * @example OneDigit.ino
 * @brief Example of Single digit LCD with 5 digits
 */

#include "SegLCD_PCF85176_OneDigit.h"

#include <Wire.h>

SegLCD_PCF85176_OneDigit lcd(Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin();

  Serial.println("Initialize LCD...");
  lcd.begin(true);

  lcd.writeString("HELLO");
  delay(2000);
}


void loop() {
  lcd.writeFloat(1);
  delay(2000);

  lcd.writeFloat(12);
  delay(2000);

  lcd.writeFloat(123);
  delay(2000);

  lcd.writeFloat(123.45, 2);
  delay(2000);

  lcd.writeFloat(3.14, 4);
  delay(2000);

  lcd.writeFloat(3.14, 2);
  delay(2000);

  lcd.writeChar(1, '1');
  lcd.writeChar(2, '2');
  lcd.writeChar(3, '3');
  lcd.writeChar(4, '4');
  lcd.writeChar(5, '5');
  delay(2000);

  lcd.clear();
  delay(500);

  lcd.writeChar(1, '2');
  lcd.writeChar(5, '3');
  lcd.setDecimal(5, true);
  delay(2000);

  lcd.setDecimal(5, false);
  lcd.setDecimal(1, true);
  delay(2000);

  lcd.clear();
  delay(2000);

  lcd.clear();
  lcd.writeFloat(3.14);
  delay(2000);

  lcd.clear();
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 10; k++) {
        lcd.writeChar(1, ((char)k + '0'));
        lcd.writeChar(2, ((char)j + '0'));
        lcd.setDecimal(2, true);
        lcd.writeChar(3, ((char)i + '0'));
        delay(100);
      }
    }
  }
  delay(2000);
}
