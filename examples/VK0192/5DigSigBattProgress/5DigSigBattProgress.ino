/**
 * @example VK0192/5DigSigBattProgress.ino
 * @brief Example of 5 digit signal, battery and progress display.
 */

#include "SegLCD_VK0192_5DigSigBattProgress.h"

#define PIN_CS 27
#define PIN_WR 14
#define PIN_DATA 12
#define PIN_PWR 13 // If you have connected Vcc on GPIO instead power line else -1

SegLCD_VK0192_5DigSigBattProgress lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  if (PIN_PWR > -1) {
    pinMode(PIN_PWR, OUTPUT);
    digitalWrite(PIN_PWR, HIGH);
  }

  lcd.init();

  // Clear LCD
  lcd.clear();

  //uint8_t _buffer[2] = {0x00, 0x60};
  //lcd.writeRam(_buffer, sizeof(_buffer), 0);

  lcd.setCursor(0,0);
  lcd.print("12342");

  lcd.setCursor(1,0);
  lcd.print(" 2340");
  lcd.setDecimal(1, 2, true);
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
