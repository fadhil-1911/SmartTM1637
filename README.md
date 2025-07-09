# SmartTM1637

SmartTM1637 is an Arduino library for controlling TM1637-based 4-digit 7-segment displays.  
It supports advanced features such as decimal points, text, time display, and brightness control.

---

## Key Features

- Display numbers with optional leading zeros  
- Display text and special symbols  
- Display time in hour:minute format with optional colon  
- Manual and automatic decimal point control  
- Brightness control (0-7 levels)  
- Support for uppercase, lowercase letters, numbers, and special characters  
- Simple and intuitive API for easy integration  

---

## System Requirements

- **Hardware:**
  - Arduino board (Uno, Nano, Mega, or compatible)
  - TM1637 4-digit 7-segment display module
  - Connection pins (CLK and DIO) properly wired to Arduino digital pins

- **Libraries:**
  - No external dependencies; this library is self-contained 
---

## Installation

1. Download the ZIP file from GitHub
2. Extract it into Documents/Arduino/libraries/MyDHT22
3. Restart the Arduino IDE
4. Open File > Examples > SmartTM1637 > BasicRead

---

## Example

```cpp
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

  display.printTime(12, 34, true);  // Show time with colon (12:34)
  delay(2000);

  display.clear();  // Clear display
  delay(1000);
}

