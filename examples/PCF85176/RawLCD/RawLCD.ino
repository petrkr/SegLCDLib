/**
 * @example PCF85176/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using PCF85176
 */

#include "SegLCD_PCF85176_Raw.h"

#include <Wire.h>

SegLCD_PCF85176_Raw lcd(Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin(0, 1);

  Serial.println("Initialize LCD...");

  // Demo for static one segment LCD display
  lcd.init(MODE_DRIVE_STATIC, MODE_BIAS_13);

  // Clear LCD
  lcd.clear();

  delay(1000);

  // Write at address 0 one byte
  lcd.write((uint8_t)0b10000000, 0);

  delay(1000);

  // Write at address 8 one byte
  lcd.write((uint8_t)0b01000000, 8);

  delay(1000);

  // Write few bytes at address 0
  uint8_t data[2] = { 0b11111111, 0b11111111 };  
  lcd.write(data, sizeof(data), 0);
}


void loop() {

}
