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
}


void loop() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      lcd.writeChar(1, ((char)i + '0'));
      lcd.writeChar(2, ((char)j + '0'));
      delay(100);
    }
  }
}
