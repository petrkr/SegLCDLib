/**
 * @example PCF8576/4Seg6SegMaintSegBatUnits/4Seg6SegMaintSegBatUnits.ino
 * @brief Example for 4+6 digit LCD with maintenance, battery, signal, units
 */

#include "SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h"
#include <Wire.h>

SegLCD_PCF8576_4Seg6SegMaintSegBatUnits lcd(Wire);

void setup() {
  Wire.begin(1, 2);
  lcd.init();
  lcd.clear();

  lcd.setSignalLevel(3);
  lcd.setBatteryLevel(4);
  lcd.setMaintenance(true);
  lcd.setLabels(SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::LABEL_MPA);

  lcd.setCursor(0, 0);
  lcd.print("1.2:3");

  lcd.setCursor(1, 0);
  lcd.print("12.3:45");
}


void loop() {

}
