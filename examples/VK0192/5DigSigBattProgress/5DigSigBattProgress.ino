/**
 * @example VK0192/5DigSigBattProgress/5DigSigBattProgress.ino
 * @brief Example of 5 digit signal, battery and progress display.
 */

#include "SegLCD_VK0192_5DigSigBattProgress.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21
#define PIN_PWR 15 // If you have connected Vcc on GPIO instead power line else -1

SegTransport3WireArduino transport(PIN_DATA, PIN_WR);
SegLCD_VK0192_5DigSigBattProgress lcd(transport, PIN_CS);

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
  lcd.setSignalLevel(4);
  delay(1000);
  lcd.setCursor(2, 0);
  lcd.print(" . . . .");
  delay(1000);
  lcd.setCursor(0, 3);
  lcd.print("12");
  delay(1000);

  lcd.setCursor(2, 0);
  lcd.print("12.34.5");
  delay(2000);
  lcd.setCursor(2, 2);
  lcd.print("5"); // Should persist dot at digit 2

  delay(2000);
  lcd.setCursor(2, 2);
  lcd.print("88"); // Should persist dot at digit 2 and remove dot at digit 3 and 4

  delay(2000);
  lcd.setCursor(2, 0);
  lcd.print("34"); // Should remove dot at digit 1

  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(" . . . .");

  delay(2000);

  lcd.setCursor(0, 0);
  lcd.print("12.356");

  lcd.setCursor(1, 0);
  lcd.print("12.3.4.5");

  delay(2000);
  lcd.setCursor(0, 2);
  lcd.print("5"); // Should persist dot at digit 2

  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("3"); // Should remove dot at digit 2

  delay(2000);
  lcd.setCursor(1, 2);
  lcd.print("88"); // Should persist dot at digit 2 and remove dot at digit 3 and 4

  delay(2000);
  lcd.setCursor(1, 0);
  lcd.print("34"); // Should remove dot at digit 1

  delay(2000);
  lcd.setDegreeSymbol(true);

  lcd.setCursor(2, 0);
  lcd.print("H.e.l.l.o");

  delay(1000);

  for (int i=0; i <= 150; i+=10) {
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
