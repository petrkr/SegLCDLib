/**
 * @example PCF85176/4DR821B/4DR821B.ino
 * @brief Example of Tesla 4DR821B LCD display using PCF85176
 */

#include "SegLCD_PCF85176_4DR821B.h"

#include <Wire.h>

SegLCD_PCF85176_4DR821B lcd(Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin(0, 1);

  Serial.println("Initialize LCD...");

  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  lcd.print("12:54");
}


void loop() {

}
