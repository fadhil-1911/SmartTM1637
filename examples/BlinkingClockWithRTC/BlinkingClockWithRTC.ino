/*
  File: BlinkingClockWithRTC.ino
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

#include <Wire.h> // Communication library
#include <RTClib.h> // RTC clock modul library
#include <SmartTM1637.h> // SmartTM1637 library

// Set pin for TM1637
SmartTM1637 display(2, 3);

// RTC object
RTC_DS3231 rtc;

// Colon status (blinking every second)
bool colonState = true;

void setup() {
  Wire.begin();
  rtc.begin();
  display.begin();
  display.setBrightness(5); // 0 = low, 7 = high

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set masa dari PC jika RTC reset
  }
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();

  display.printTime(hour, minute, colonState); // Print Time
  colonState = !colonState; // Change colon status (true/false)

  delay(1000); // 
}
