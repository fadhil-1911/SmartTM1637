#include "AutoTM1637Display.h"
#include <MyDHT22.h>

// Pin sambungan
#define DHT_PIN 7    // D4 (data pin DHT22)
#define CLK_PIN_1 2  // D2 (CLK TM1637)
#define DIO_PIN_1 3  // D3 (DIO TM1637)
#define CLK_PIN_2 4  // D4 (CLK TM1637)
#define DIO_PIN_2 5  // D5 (DIO TM1637)

AutoTM1637Display display_1(CLK_PIN_1, DIO_PIN_1);
AutoTM1637Display display_2(CLK_PIN_2, DIO_PIN_2);
MyDHT22 dht(DHT_PIN);

void setup() {
  Serial.begin(9600);
  display_1.begin(0);  // brightness maksimum
  display_2.begin(0);  // brightness maksimum
  display_1.print("init");
  delay(1000);
}

void loop() {
  if (dht.readData()) {
    float suhu = dht.getTemperature();
    int kelembapan = dht.getHumidity();

    //=========================================================
    // Format suhu ke string untuk 4-digit display, contoh "25*C"
    //char buf[5];
    //snprintf(buf, sizeof(buf), "%2d*C", (int)suhu);
    //display.print(buf);
    //========================================================
    //display_1.printNumber(suhu,false);
    display_1.print(suhu, "*C");
    display_2.print(kelembapan, "rh");

    Serial.print("Temp: ");
    Serial.print(suhu);

    Serial.print(" | Humadity: ");
    Serial.println(kelembapan);

    delay(2000);

  } else {
    display_1.print("Err");
  }
  //============================================================
  // Papar nombor 0
  //display.printNumber(22, true); // true  0022 / falae 22

  /*for (int i = 0; i <= 1000; i += 5) {
  display.printNumber(i, false); // papar 0005, 0010, 0015, ...
  delay(100); 
} */
  //=======================================================

  //delay(2000);  // Baca sensor setiap 2 saat
}


/* next update
versi printFloat(float n, uint8_t precision)

versi printTime(int hour, int min) dengan colon

atau scrollText() untuk mesej panjang
*/
