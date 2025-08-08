/**
 * @example 6DigSigBattProgress.ino
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

  lcd.init();

  // Clear LCD
  lcd.clear();
  lcd.print("31:40");
  lcd.setCursor(1, 0);
  lcd.print("1.2.3.4:5.0");
  delay(2000);

  lcd.home();
  lcd.print("12:3 ");
  lcd.setCursor(1, 0);
  lcd.print("567890");
  delay(1000);

  lcd.setCursor(1, 2);
  lcd.print("12:24");

  delay(2000);

  lcd.setLabels(SegLCD_PCF85176_6DigitSignalBatteryProgress::LABEL_MPA);
  lcd.setCursor(1, 0);
  lcd.print("hel.l.o. ");

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
