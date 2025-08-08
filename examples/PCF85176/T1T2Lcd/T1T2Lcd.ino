/**
 * @example PCF85176/T1T2Lcd/T1T2Lcd.ino
 * @brief Example of T1, T2 LCD
 */

#include "SegLCD_PCF85176_T1T2Lcd.h"

#include <Wire.h>

SegLCD_PCF85176_T1T2Lcd lcd(Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  Wire.begin(0, 1);

  Serial.println("Initialize LCD...");

  // Demo for duty 1/4, bias 1/3 LCD display
  lcd.init();

  // Clear LCD
  lcd.clear();
  delay(500);

  lcd.setCursor(1, 0);
  lcd.print("-3.14");
  delay(1000);

  lcd.setCursor(2, 0);
  lcd.print("2.4");
  delay(1000);

  lcd.setClockSymbol(true);
  delay(1000);

  // Clock
  lcd.setCursor(0, 0);
  lcd.print("1224");

  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("4:77");
  lcd.print("5");

  // Day of week
  lcd.print("2");

  delay(500);

  lcd.setLabels(SegLCD_PCF85176_T1T2Lcd::LABEL_T2_HUM | SegLCD_PCF85176_T1T2Lcd::LABEL_T1_C_UP);
  lcd.setCursor(1, 0);
  lcd.print("h.e.l.o");

  lcd.setCursor(2, 0);
  lcd.print("HELO");

  lcd.setT1T2Labels(SegLCD_PCF85176_T1T2Lcd::LABEL_T1);
  delay(500);

  lcd.setT1T2Labels(SegLCD_PCF85176_T1T2Lcd::LABEL_T2);
  delay(1000);

  lcd.clearT1T2Labels(SegLCD_PCF85176_T1T2Lcd::LABEL_T2);
  
}

void loop() {
  // Example Signal level
  lcd.setSignalLevel(0);delay(500);
  lcd.setSignalLevel(1);delay(500);
  lcd.setSignalLevel(2);delay(500);
  lcd.setSignalLevel(3);delay(500);
  lcd.setSignalLevel(4);delay(500);
  lcd.setSignalLevel(5);delay(500);

  // Example Battery level

  lcd.setBatteryLevel(0);delay(500);
  lcd.setBatteryLevel(1);delay(500);
  lcd.setBatteryLevel(2);delay(500);
  lcd.setBatteryLevel(3);delay(500);
  lcd.setBatteryLevel(4);delay(500);
}
