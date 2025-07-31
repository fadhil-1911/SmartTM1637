/***************************************************************
*  File Name    : SmartTM1637.cpp
*  Description  : Driver for the TM1637 4-digit 7-segment display
*                 with extended features such as decimal point,
*                 text, time display, and brightness control.
*
*  Version      : 1.0
*  Date         : 9 July 2025
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

#include "SmartTM1637.h"

SmartTM1637::SmartTM1637(uint8_t clk, uint8_t dio)
  : _clkPin(clk), _dioPin(dio), _brightness(7) {}

void SmartTM1637::begin(uint8_t brightness) {
  pinMode(_clkPin, OUTPUT);
  pinMode(_dioPin, OUTPUT);
  _brightness = brightness;
  clear();
}

void SmartTM1637::start() {
  digitalWrite(_clkPin, HIGH);
  digitalWrite(_dioPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(_dioPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_clkPin, LOW);
}

void SmartTM1637::stop() {
  digitalWrite(_clkPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_dioPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_clkPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(_dioPin, HIGH);
}

void SmartTM1637::writeByte(uint8_t b) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, (b >> i) & 0x01);
    delayMicroseconds(5);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
  }

  // optional: read ACK
  digitalWrite(_clkPin, LOW);
  pinMode(_dioPin, INPUT);
  delayMicroseconds(5);
  digitalWrite(_clkPin, HIGH);
  delayMicroseconds(5);
  pinMode(_dioPin, OUTPUT);
}

void SmartTM1637::setBrightness(uint8_t brightness) {
  _brightness = brightness & 0x07;
  start();
  writeByte(0x88 | _brightness);
  stop();
}

void SmartTM1637::setSegments(const uint8_t segs[4]) {
  start();
  writeByte(0x40);  // Set data command
  stop();

  start();
  writeByte(0xC0);  // Start address
  for (uint8_t i = 0; i < 4; i++) {
    writeByte(segs[i]);
  }
  stop();

  setBrightness(_brightness);
}

void SmartTM1637::clear() {
  uint8_t blank[4] = { 0, 0, 0, 0 };
  setSegments(blank);
}

//================================================
//          display_1.print("init");
//================================================
void SmartTM1637::print(const char* text) {
  uint8_t seg[4] = { 0, 0, 0, 0 };
  for (uint8_t i = 0; i < 4 && text[i] != '\0'; i++) {
    seg[i] = encodeChar(text[i]);
  }
  setSegments(seg);
}

//============================================================================
//  display.printNumber(22, true); // true  0022 / falae 22
//============================================================================
void SmartTM1637::printNumber(int num, bool leadingZero) {
  uint8_t seg[4] = { 0, 0, 0, 0 };

  num = constrain(num, 0, 9999);
  int d[4] = {
    (num / 1000) % 10,
    (num / 100) % 10,
    (num / 10) % 10,
    num % 10
  };

  bool leading = !leadingZero;

  for (int i = 0; i < 4; i++) {
    if (d[i] != 0 || i == 3 || leadingZero) {
      seg[i] = encodeChar('0' + d[i]);
      leadingZero = true; // issue #1 by ajwhale fixed
    } else if (!leading) {
      seg[i] = 0x00;
    }
  }

  setSegments(seg);  
}

//==========================================================
//    display_1.print(temp, "*C");
//==========================================================
void SmartTM1637::print(int val, const char* suffix) {
  char buf[5] = { ' ', ' ', ' ', ' ', '\0' };

  if (val < 10) {
    buf[0] = ' ';
    buf[1] = '0' + val;
  } else if (val < 100) {
    buf[0] = '0' + (val / 10);
    buf[1] = '0' + (val % 10);
  } else {
    buf[0] = '*';
    buf[1] = '*';  // overflow
  }

  buf[2] = suffix[0];  // Example: '*'
  buf[3] = suffix[1];  // Example: 'C'

  print(buf);  
}

//========================================================================
//                   printWithDots
//========================================================================
void SmartTM1637::printWithDots(const char* text, uint8_t dotMask) {
  uint8_t seg[4] = { 0, 0, 0, 0 };
  for (uint8_t i = 0; i < 4 && text[i] != '\0'; i++) {
    seg[i] = encodeChar(text[i]);
    if (dotMask & (1 << (3 - i))) {
      seg[i] |= 0x80;  // Bit 7 = dot
    }
  }
  setSegments(seg);
}

//===============================================================================================================
// Version 1 – Beginner (use showDecimal) display_1.print(temp, "C", true); // Beginner → 25.5C (dot at digit 2)
//===============================================================================================================
void SmartTM1637::print(float val, const char* suffix, bool showDecimal) {
  print(val, suffix, false, showDecimal, false, false);
}

//===============================================================================================================
// Version 2 – Pro (manual dot control) display_1.print(temp, "C", false, true, false, false);  // Pro → 25.5C
//===============================================================================================================
void SmartTM1637::print(float val, const char* suffix,
                              bool dot1, bool dot2, bool dot3, bool dot4) {
  char buf[5] = { ' ', ' ', ' ', ' ', '\0' };
  uint8_t dotMask = 0;

  if (dot1) dotMask |= 0b1000;
  if (dot2) dotMask |= 0b0100;
  if (dot3) dotMask |= 0b0010;
  if (dot4) dotMask |= 0b0001;

  int intVal = int(val * 10.0f);  // one decimal place
  bool isNegative = intVal < 0;
  if (isNegative) intVal = -intVal;

  int d0 = (intVal / 100) % 10;
  int d1 = (intVal / 10) % 10;
  int d2 = intVal % 10;

  if (intVal > 999) {
    buf[0] = '*';
    buf[1] = '*';
    buf[2] = suffix[0];
    buf[3] = suffix[1];
  } else {
    buf[0] = '0' + d0;
    buf[1] = '0' + d1;
    buf[2] = '0' + d2;
    buf[3] = suffix[0];  // Example: 'C'
  }

  printWithDots(buf, dotMask);
}

//====================================================================================
//.       display_1.print(suhu, false, false, true, false);  = 34.50
//====================================================================================
void SmartTM1637::print(float val, bool dot1, bool dot2, bool dot3, bool dot4) {
  uint8_t seg[4] = { 0, 0, 0, 0 };
  uint8_t dotMask = 0;

  if (dot1) dotMask |= 0b1000;
  if (dot2) dotMask |= 0b0100;
  if (dot3) dotMask |= 0b0010;
  if (dot4) dotMask |= 0b0001;

  // Convert to integer with 2 decimal points
  int intVal = int(val * 100.0f);  // 34.40 → 3440
  intVal = constrain(intVal, 0, 9999);

  int d[4] = {
    (intVal / 1000) % 10,
    (intVal / 100) % 10,
    (intVal / 10) % 10,
    intVal % 10
  };

  for (int i = 0; i < 4; i++) {
    seg[i] = encodeChar('0' + d[i]);
    if (dotMask & (1 << (3 - i))) {
      seg[i] |= 0x80;
    }
  }

  setSegments(seg);
}

//===========================================================================================
//               display_2.print("v100", false, true, true, false);
//===========================================================================================
void SmartTM1637::print(const char* text, bool dot1, bool dot2, bool dot3, bool dot4) {
  uint8_t dotMask = 0;
  if (dot1) dotMask |= 0b1000;
  if (dot2) dotMask |= 0b0100;
  if (dot3) dotMask |= 0b0010;
  if (dot4) dotMask |= 0b0001;
  printWithDots(text, dotMask);
}

//==========================================================================
//                         rtc modul + colon
//==========================================================================
void SmartTM1637::printTime(uint8_t hour, uint8_t minute, bool showColon) {
  uint8_t seg[4];

  hour = constrain(hour, 0, 99);
  minute = constrain(minute, 0, 99);

  seg[0] = encodeChar('0' + (hour / 10));
  seg[1] = encodeChar('0' + (hour % 10));
  seg[2] = encodeChar('0' + (minute / 10));
  seg[3] = encodeChar('0' + (minute % 10));

  if (showColon) {
    seg[1] |= 0x80;  // Bit 7 = midpoint between hour & minute
  }

  setSegments(seg);
}

//=============================================
uint8_t SmartTM1637::encodeChar(char c) {
  switch (toupper(c)) {
      // Digit 0–9

    case '0': return 0b00111111;
    case '1': return 0b00000110;
    case '2': return 0b01011011;
    case '3': return 0b01001111;
    case '4': return 0b01100110;
    case '5': return 0b01101101;
    case '6': return 0b01111101;
    case '7': return 0b00000111;
    case '8': return 0b01111111;
    case '9': return 0b01101111;

    //HURUF KECIL
    case 'a': return 0b01110111;
    case 'b': return 0b01111100;
    case 'c': return 0b01011000;
    case 'd': return 0b01011110;
    case 'e': return 0b01111001;
    case 'f': return 0b01110001;
    case 'g': return 0b01101111;
    case 'h': return 0b01110100;
    case 'i': return 0b00000100;
    case 'j': return 0b00011110;
    case 'k': return 0b01110110;
    case 'l': return 0b00111000;
    case 'm': return 0b01010101;
    case 'n': return 0b01010100;
    case 'o': return 0b01011100;
    case 'p': return 0b01110011;
    case 'q': return 0b01100111;
    case 'r': return 0b01010000;
    case 's': return 0b01101101;
    case 't': return 0b01111000;
    case 'u': return 0b00011100;
    case 'v': return 0b00111110;
    case 'w': return 0b00101010;
    case 'x': return 0b01110110;
    case 'y': return 0b01101110;
    case 'z': return 0b01011011;


    // HURUF BESAR
    case 'A': return 0b01110111;
    case 'B': return 0b01111100;
    case 'C': return 0b00111001;
    case 'D': return 0b01011110;
    case 'E': return 0b01111001;
    case 'F': return 0b01110001;
    case 'G': return 0b00111101;
    case 'H': return 0b01110110; //0b01110100
    case 'I': return 0b00000110;
    case 'J': return 0b00011110;
    case 'K': return 0b01110110;
    case 'L': return 0b00111000;
    case 'M': return 0b00110111;
    case 'N': return 0b00110111;
    case 'O': return 0b00111111;
    case 'P': return 0b01110011;
    case 'Q': return 0b01100111;
    case 'R': return 0b01010000;
    case 'S': return 0b01101101;
    case 'T': return 0b01111000;
    case 'U': return 0b00111110;
    case 'V': return 0b00111110;
    case 'W': return 0b00101010;
    case 'X': return 0b01110110;
    case 'Y': return 0b01101110;
    case 'Z': return 0b01011011;


    case '-': return 0b01000000;
    case '_': return 0b00001000;
    case '*':
    case '°': return 0b01100011;
    case ' ': return 0b00000000;
    default: return 0b00000000;
  }
}

