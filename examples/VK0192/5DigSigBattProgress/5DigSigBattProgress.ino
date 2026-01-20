/**
 * @example VK0192/5DigSigBattProgress/5DigSigBattProgress.ino
 * @brief Example of 5 digit signal, battery and progress display.
 */

#include "SegLCD_VK0192_5DigSigBattProgress.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21
#define PIN_PWR 15 // If you have connected Vcc on GPIO instead power line else -1

SegLCD_VK0192_5DigSigBattProgress lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  if (PIN_PWR > -1) {
    lcd.initBacklight(PIN_PWR);
    lcd.setBacklight(true);
  }

  lcd.init();

  // Clear LCD
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("1.2.3");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("5");

  lcd.setCursor(1, 0);
  lcd.print("12.3.4.5");
  delay(1000);
  lcd.setCursor(1, 2);
  lcd.print("67");

  lcd.setLabels(SegLCD_VK0192_5DigSigBattProgress::LABEL_C);

  lcd.setCursor(2, 0);
  lcd.print("Hello");


  delay(1000);
  //lcd.clearLabels(SegLCD_VK0192_5DigSigBattProgress::LABEL_M);

  for (int i=0; i <= 100; i+=10) {
    lcd.setProgress(i);
    delay(500);
  }

  delay(1000);

}

void loop() {
  for (int i=0; i < 5; i++) {
    lcd.setBatteryLevel(i);
    delay(1000);
  }

  for (int i=0; i < 4; i++) {
    lcd.setSignalLevel(i);
    delay(1000);
  }
}
