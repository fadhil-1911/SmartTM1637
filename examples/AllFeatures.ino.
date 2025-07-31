#include <SmartTM1637.h>

// CLK = D2, DIO = D3
SmartTM1637 display(2, 3);

void setup() {
  display.begin();             // Wajib panggil untuk inisialisasi
  display.setBrightness(7);    // Set kecerahan (0–7)

  delay(1000);

  // 1️⃣ print(const char* text)
  display.print("init");
  delay(2000);

  // 2️⃣ print(int number, bool leadingZero = false)
  display.print(42);           // Papar: "  42"
  delay(1000);
  display.print(42, true);     // Papar: "0042"
  delay(1000);

  // 3️⃣ print(int number, const char* suffix)
  display.print(25, "*C");     // Papar: "25*C"
  delay(1000);
  display.print(-10, "C");     // Papar: "-10C"
  delay(1000);

  // 4️⃣ print(float number, const char* suffix, bool autoDot = false)
  display.print(3.14, "", true);   // Papar: "3.14"
  delay(1000);
  display.print(25.5, "C", true);  // Papar: "25.5C"
  delay(1000);

  // 5️⃣ print(const char* text, uint8_t dotMask)
  // dotMask = 0bDP4 DP3 DP2 DP1 (bit 3–0)
  display.print("1234", 0b0101);   // Papar: "1.2.3.4"
  delay(1000);

  // 6️⃣ printTime(uint8_t hour, uint8_t minute, bool colon = true)
  display.printTime(12, 45, true);  // Papar: "12:45"
  delay(1000);
  display.printTime(9, 7, false);   // Papar: "0907" (tanpa titik tengah)
  delay(1000);

  // 7️⃣ printRaw(uint8_t rawBytes[4])
  // Manual kawalan semua 4 digit
  uint8_t raw[] = { 0x3f, 0x06, 0x5b, 0x4f };  // Papar: "0123"
  display.printRaw(raw);
  delay(1000);

  // 8️⃣ clear()
  display.clear();     // Kosongkan paparan
}

void loop() {
  // Kosongkan atau animasi ulangan di sini
}
