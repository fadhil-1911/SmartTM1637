
#include "AutoTM1637Display.h"

AutoTM1637Display::AutoTM1637Display(uint8_t clk, uint8_t dio)
  : _clkPin(clk), _dioPin(dio), _brightness(7) {}

void AutoTM1637Display::begin(uint8_t brightness) {
  pinMode(_clkPin, OUTPUT);
  pinMode(_dioPin, OUTPUT);
  _brightness = brightness;
  clear();
}

void AutoTM1637Display::start() {
  digitalWrite(_clkPin, HIGH);
  digitalWrite(_dioPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(_dioPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_clkPin, LOW);
}

void AutoTM1637Display::stop() {
  digitalWrite(_clkPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_dioPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_clkPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(_dioPin, HIGH);
}

void AutoTM1637Display::writeByte(uint8_t b) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, (b >> i) & 0x01);
    delayMicroseconds(3);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(3);
  }

  // optional: read ACK
  digitalWrite(_clkPin, LOW);
  pinMode(_dioPin, INPUT);
  delayMicroseconds(3);
  digitalWrite(_clkPin, HIGH);
  delayMicroseconds(3);
  pinMode(_dioPin, OUTPUT);
}

void AutoTM1637Display::setBrightness(uint8_t brightness) {
  _brightness = brightness & 0x07;
  start();
  writeByte(0x88 | _brightness);
  stop();
}

void AutoTM1637Display::setSegments(const uint8_t segs[4]) {
  start();
  writeByte(0x40); // Set data command
  stop();

  start();
  writeByte(0xC0); // Start address
  for (uint8_t i = 0; i < 4; i++) {
    writeByte(segs[i]);
  }
  stop();

  setBrightness(_brightness);
}

void AutoTM1637Display::clear() {
  uint8_t blank[4] = {0, 0, 0, 0};
  setSegments(blank);
}

void AutoTM1637Display::print(const char* text) {
  uint8_t seg[4] = {0, 0, 0, 0};
  for (uint8_t i = 0; i < 4 && text[i] != '\0'; i++) {
    seg[i] = encodeChar(text[i]);
  }
  setSegments(seg);
}

// fungsi tambahan 1
void AutoTM1637Display::printNumber(int num, bool leadingZero) {
  uint8_t seg[4] = {0, 0, 0, 0};

  num = constrain(num, 0, 9999);
  int d[4] = {
    (num / 1000) % 10,
    (num / 100)  % 10,
    (num / 10)   % 10,
    num % 10
  };

  bool leading = !leadingZero;

  for (int i = 0; i < 4; i++) {
    if (d[i] != 0 || i == 3 || leadingZero) {
      seg[i] = encodeChar('0' + d[i]);
      leading = true;
    } else if (!leading) {
      seg[i] = 0x00;
    }
  }

  setSegments(seg); // gunakan fungsi kita sendiri
}

// fungsi tambahan 2
void AutoTM1637Display::print(int val, const char* suffix) {
  char buf[5] = {' ', ' ', ' ', ' ', '\0'};

  if (val < 10) {
    buf[0] = ' ';
    buf[1] = '0' + val;
  } else if (val < 100) {
    buf[0] = '0' + (val / 10);
    buf[1] = '0' + (val % 10);
  } else {
    buf[0] = '*'; buf[1] = '*'; // overflow
  }

  buf[2] = suffix[0]; // contoh: '*'
  buf[3] = suffix[1]; // contoh: 'C'

  print(buf); // guna fungsi print(const char*)
}




uint8_t AutoTM1637Display::encodeChar(char c) {
  switch (toupper(c)) {
        // Digit 0â9

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
    case 'H': return 0b01110110;
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
    case 'Â°': return 0b01100011;
    case ' ': return 0b00000000;
    default: return 0b00000000; 
  }
}
