/***************************************************************
*  File Name    : SmartTM1637.h
*  Description  : Driver for the TM1637 4-digit 7-segment display
*                 with extended features such as decimal point,
*                 text, time display, and brightness control.
*
*  Version      : 1.0.1
*  Date         : 01 August 2025
*  Platform     : Arduino (Uno/Nano/Mega, etc.)
*  Author       : [fadhil /  SmartTM1637]
*
*  This library supports:
*   - Number display with or without leading zeros
*   - Text and symbol display
*   - Time display (HH:MM)
*   - Decimal point control (manual & automatic)
*   - Support for uppercase/lowercase letters and special characters
*
*  Notes:
*   - 
*   - 
****************************************************************/
#ifndef SMARTTM1637DISPLAY_H
#define SMARTTM1637DISPLAY_H

#include <Arduino.h>

class SmartTM1637 {
public:
  SmartTM1637(uint8_t clk, uint8_t dio);
  void begin(uint8_t brightness = 7);
  void print(const char* text);

  // additional function 1
  void printNumber(int num, bool leadingZero = false);
  // fungsi tambahan 2
  void print(int val, const char* suffix);

  // additional function 1 3 01JULY25
  void print(float val, const char* suffix, bool showDecimal = false);                    // Beginner
  void print(float val, const char* suffix, bool dot1, bool dot2, bool dot3, bool dot4);  // Pro
  void printWithDots(const char* text, uint8_t dotMask = 0);                              // internal Utiliti 

  // additional function 1 4 01JULY25
  void print(float val, bool dot1, bool dot2, bool dot3, bool dot4);

  // additional function 1 5 01JULY25
  void print(const char* text, bool dot1, bool dot2, bool dot3, bool dot4);  // Beginner-friendly overload

  void printTime(uint8_t hour, uint8_t minute, bool showColon = true);

  // additional function 1 6 02JULY25
  //void printWithDots(const char* text, uint8_t dotMask = 0);
  void clear();

private:
  uint8_t _clkPin, _dioPin, _brightness;

  void start();
  void stop();
  void writeByte(uint8_t b);
  void setSegments(const uint8_t segs[4]);
  void setBrightness(uint8_t brightness);
  uint8_t encodeChar(char c);
};

#endif






