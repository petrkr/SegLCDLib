/**
 * @example TempHumidity.ino
 * @brief Example of Temperature and Humidity LCD
 */

#include "SegLCD_PCF85176_TempHum.h"
#include <Wire.h>

SegLCD_PCF85176_TempHumidity lcd(Wire);

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

  lcd.writeFloat(61.1, 1, SegLCD_PCF85176_TempHumidity::LCDSections::SECTION_HUMIDITY);
  delay(1000);

  lcd.writeChar(1, '1');
  lcd.writeChar(2, '2');
  lcd.writeChar(3, '3');
  lcd.writeChar(4, '4');
  lcd.setLabels(SegLCD_PCF85176_TempHumidity::LABEL_C);
  delay(1000);

  lcd.writeChar(1, '5', SegLCD_PCF85176_TempHumidity::LCDSections::SECTION_HUMIDITY);
  lcd.writeChar(2, '6', SegLCD_PCF85176_TempHumidity::LCDSections::SECTION_HUMIDITY);
  lcd.writeChar(3, '7', SegLCD_PCF85176_TempHumidity::LCDSections::SECTION_HUMIDITY);
  lcd.setLabels(SegLCD_PCF85176_TempHumidity::LABEL_PROC);

  delay(2000);
  lcd.clearLabels(SegLCD_PCF85176_TempHumidity::LABEL_C | SegLCD_PCF85176_TempHumidity::LABEL_PROC);

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
