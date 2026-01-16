/**
 * @example PCF85176/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using PCF85176
 */

#include "SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h"

#include <Wire.h>

SegLCD_PCF8576_4Seg6SegMaintSegBatUnits lcd(Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin(1, 2);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.println("Initialize LCD...");

  // Demo for static one segment LCD display
  lcd.init();

  // Clear LCD
  lcd.clear();

  lcd.setSignalLevel(1);
  delay(1000);
  lcd.setSignalLevel(2);
  delay(1000);
  lcd.setSignalLevel(3);
  delay(1000);
  lcd.setSignalLevel(4);
  delay(1000);
  lcd.setSignalLevel(0);

  delay(1000);
  lcd.print("5");
  lcd.print("6");
  lcd.print("7");
  lcd.print("8");

  lcd.setCursor(1, 0);
  lcd.print("1");
  lcd.print("2");
  lcd.print("3");
  lcd.print("4");
  lcd.print("5");
  lcd.print("6");

  return;
  delay(1000);

  // Write at address 0 one byte
  lcd.writeRam((uint8_t)0xF0, 0);

  delay(1000);

  // Write at address 8 one byte
  lcd.writeRam((uint8_t)0xF0, 8);
}


void loop() {

}
