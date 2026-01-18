/**
 * @example PCF8576/4Seg6SegMaintSegBatUnits/4Seg6SegMaintSegBatUnits.ino
 * @brief Example for 4+6 digit LCD with maintenance, battery, signal, units
 */

#include "SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h"
#include <Wire.h>

SegLCD_PCF8576_4Seg6SegMaintSegBatUnits lcd(Wire);

void setup() {
  Wire.begin(1, 2);
  lcd.initBacklight(10, SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::BACKLIGHT_PWM);
  lcd.setBacklight(128);

  lcd.init();
  lcd.clear();

  lcd.setSignalLevel(3);
  lcd.setBatteryLevel(2);
  lcd.setMaintenance(true);
  //lcd.setLabels(SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::LABEL_);
  lcd.setDegree(true);

  lcd.setCursor(0, 0);
  lcd.print("17:37");

  lcd.setCursor(1, 0);
  lcd.print("T1:-3.41");
}


void loop() {

}
