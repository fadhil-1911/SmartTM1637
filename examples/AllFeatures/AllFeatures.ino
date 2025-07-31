/*
  File: AllFeatures.ino
  Description: Simple example sketch to demonstrate usage of SmartTM1637 library
               for TM1637 4-digit 7-segment display.
  Version: v1.0.0
  Date: 9 July 2025
  Author: [fadhil1911]
  GitHub: https://github.com/fadhil-1911/SmartTM1637
  Wiring:
    CLK = D2 (Pin D2)
    DIO = D3 (Pin D3)
*/


#include <SmartTM1637.h>

// CLK = Pin D2, DIO = Pin D3
SmartTM1637 display(2, 3);

void setup() {
  display.begin();             // Must call for initialization
  display.setBrightness(7);    // Set the brightness (0–7)

  delay(1000);

  // 1️⃣ print(const char* text)
  display.print("init");
  delay(2000);

  // 2️⃣ print(int number, bool leadingZero = false)
  display.print(42);           // Show: "  42"
  delay(1000);
  display.print(42, true);     // Show: "0042"
  delay(1000);

  // 3️⃣ print(int number, const char* suffix)
  display.print(25, "*C");     // Show: "25*C"
  delay(1000);
  display.print(-10, "C");     // Show: "-10C"
  delay(1000);

  // 4️⃣ print(float number, const char* suffix, bool autoDot = false)
  display.print(3.14, "", true);   // Show: "3.14"
  delay(1000);
  display.print(25.5, "C", true);  // Show: "25.5C"
  delay(1000);

  // 5️⃣ print(const char* text, uint8_t dotMask)
  // dotMask = 0bDP4 DP3 DP2 DP1 (bit 3–0)
  display.print("1234", 0b0101);   // Show: "1.2.3.4"
  delay(1000);

  // 6️⃣ printTime(uint8_t hour, uint8_t minute, bool colon = true)
  display.printTime(12, 45, true);  // Show: "12:45"
  delay(1000);
  display.printTime(9, 7, false);   // Show: "0907" (without colon)
  delay(1000);

  // 7️⃣ printRaw(uint8_t rawBytes[4])
  // Manual control for all 4 digit
  uint8_t raw[] = { 0x3f, 0x06, 0x5b, 0x4f };  // Papar: "0123"
  display.printRaw(raw);
  delay(1000);

  // 8️⃣ clear()
  display.clear();     // Clear the display
}

void loop() {

}
