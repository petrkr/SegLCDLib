/**
 * @example 6DigSigBattProgres.ino
 * @brief Example of 6 Digit Signal Battery Progress LCD
 */

#include "SegLCD_PCF85176_6DigSigBattProgress.h"
#include <Wire.h>

SegLCD_PCF85176_6DigitSignalBatteryProgress lcd(Wire);


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin(0, 1);

  Serial.println("Initialize LCD...");

  lcd.begin();

  // Clear LCD
  lcd.clear();
  delay(500);

  lcd.writeFloat(-3.14, 2);
  delay(1000);

  lcd.writeFloat(21.1, 1, SegLCD_PCF85176_6DigitSignalBatteryProgress::LCDSections::SECTION_CLOCK);
  delay(1000);

  lcd.writeChar(1, '1', SegLCD_PCF85176_6DigitSignalBatteryProgress::LCDSections::SECTION_CLOCK);
  lcd.writeChar(2, '2', SegLCD_PCF85176_6DigitSignalBatteryProgress::LCDSections::SECTION_CLOCK);
  lcd.writeChar(3, '2', SegLCD_PCF85176_6DigitSignalBatteryProgress::LCDSections::SECTION_CLOCK);
  lcd.writeChar(4, '4', SegLCD_PCF85176_6DigitSignalBatteryProgress::LCDSections::SECTION_CLOCK);
  lcd.setClockColon(true);

  delay(1000);

  lcd.setLabels(SegLCD_PCF85176_6DigitSignalBatteryProgress::LABEL_MPA);
  lcd.writeChar(1, 'h');
  lcd.writeChar(2, 'e');
  lcd.writeChar(3, 'l');
  lcd.writeChar(4, 'l');
  lcd.writeChar(5, 'o');
  lcd.writeChar(6, ' ');

  lcd.setDecimal(1, true);
  lcd.setDecimal(2, true);
  lcd.setClockColon(true, SegLCD_PCF85176_6DigitSignalBatteryProgress::LCDSections::SECTION_DEFAULT);

  lcd.setWheel(980);
  lcd.setProgress(110);
}

void loop() {
  // Example Signal level
  lcd.setSignalLevel(1);delay(500);
  lcd.setSignalLevel(2);delay(500);
  lcd.setSignalLevel(3);delay(500);
  lcd.setSignalLevel(4);delay(500);


  // Example Battery level
  lcd.setBatteryLevel(1);delay(500);
  lcd.setBatteryLevel(2);delay(500);
  lcd.setBatteryLevel(3);delay(500);
  lcd.setBatteryLevel(4);delay(500);
}
