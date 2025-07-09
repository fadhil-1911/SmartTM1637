/*
  File: BasicSmartTM1637Example.ino
  Description: Simple example sketch to demonstrate usage of SmartTM1637 library
               for TM1637 4-digit 7-segment display.
  Date: 9 July 2025
  Author: [fadhil1911]
*/


#include <SmartTM1637.h>

#define CLK_PIN 2
#define DIO_PIN 3

SmartTM1637 display(CLK_PIN, DIO_PIN);

void setup() {
  display.begin(7);  // Initialize with max brightness
}

void loop() {
  display.print("HELP");  // Show text on display
  delay(2000);

  display.printNumber(1234, false);  // Show number without leading zeros
  delay(2000);

  display.printTime(12, 34, true);  // Show time with colon (12:34) **for segment with colon
  delay(2000);

  display.print("v1.0", false, true, false, false); // Dot after 1 **for segment with dot
  delay(2000);
  
  display.clear();  // Clear display
  delay(1000);
}
