#include <Wire.h> // 
#include <RTClib.h>
#include <SmartTM1637.h>

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
