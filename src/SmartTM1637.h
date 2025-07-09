
#ifndef AUTOTM1637DISPLAY_H
#define AUTOTM1637DISPLAY_H

#include <Arduino.h>

class AutoTM1637Display {
public:
  AutoTM1637Display(uint8_t clk, uint8_t dio);
  void begin(uint8_t brightness = 7);
  void print(const char* text);
  void printNumber(int num, bool leadingZero = false); // fungsi tambahan 1
  void print(int val, const char* suffix); // fungsi tambahan 2
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
