/**
 * @example PCF85176/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using PCF85176
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

  // Demo for static one segment LCD display
  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  // Write at address 0 one byte
  lcd.print("1254");

  lcd.setClockColon(0, 0, true);
}


void loop() {

}
