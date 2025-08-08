/**
 * @example PCF85176/TempHumidity/TempHumidity.ino
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

  lcd.init();

  // Clear LCD
  lcd.clear();
  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("-12.-3");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("---");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("2--");
  delay(1000);
  lcd.setDecimal(0, 1, true);
  lcd.setDecimal(0, 3, true);
  delay(1000);
  lcd.setCursor(1, 0);
  lcd.print("3.45");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("12.34");
  lcd.setCursor(1, 0);
  lcd.print("56.7");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("98.76");
  lcd.setCursor(1, 0);
  lcd.print("54.3");
  lcd.setLabels(SegLCD_PCF85176_TempHumidity::LABEL_C | SegLCD_PCF85176_TempHumidity::LABEL_PROC);

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
