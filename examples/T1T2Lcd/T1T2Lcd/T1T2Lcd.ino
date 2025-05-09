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
  lcd.begin();

  // Clear LCD
  lcd.clear();
  delay(500);
//lcd.write((uint8_t)0xff, 4);

  lcd.setLabels(SegLCD_PCF85176_T1T2Lcd::LABEL_T2_HUM | SegLCD_PCF85176_T1T2Lcd::LABEL_T1_C_UP);
  lcd.writeChar(1, 'h');
  lcd.writeChar(2, 'e');
  lcd.writeChar(3, 'l');
  lcd.writeChar(4, 'o');

  lcd.writeChar(1, 'H', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_T2);
  lcd.writeChar(2, 'E', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_T2);
  lcd.writeChar(3, 'L', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_T2);
  lcd.writeChar(4, 'O', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_T2);

  lcd.setT1T2Labels(SegLCD_PCF85176_T1T2Lcd::LABEL_T1);
  delay(500);

  lcd.writeChar(1, 'a', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_CLOCK);
  lcd.writeChar(2, 'h', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_CLOCK);
  lcd.writeChar(3, 'o', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_CLOCK);
  lcd.writeChar(4, 'j', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_CLOCK);

  lcd.writeChar(1, '5', SegLCD_PCF85176_T1T2Lcd::LCDSections::SECTION_DAY);

  delay(500);
  lcd.setT1T2Labels(SegLCD_PCF85176_T1T2Lcd::LABEL_T2);

  delay(1000);
  lcd.clearT1T2Labels(SegLCD_PCF85176_T1T2Lcd::LABEL_T2);
  
}

void loop() {
  // Example Signal level
/*  lcd.setSignalLevel(0);delay(500);
  lcd.setSignalLevel(1);delay(500);
  lcd.setSignalLevel(2);delay(500);
  lcd.setSignalLevel(3);delay(500);
  lcd.setSignalLevel(4);delay(500);
  lcd.setSignalLevel(5);delay(500); */

  // Example Battery level
/*
  lcd.setBatteryLevel(0);delay(500);
  lcd.setBatteryLevel(1);delay(500);
  lcd.setBatteryLevel(2);delay(500);
  lcd.setBatteryLevel(3);delay(500);
  lcd.setBatteryLevel(4);delay(500); */

  
}
