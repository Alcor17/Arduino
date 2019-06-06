/*

   kod för att styra en DLR3416-display från Siemens


   Pinout
   Arduino    DLR 3416    DLR 2416
      2          16    D0     11
      3          17    D1     12
      4          18    D2     13
      5          19    D3     14
      6          20    D4     17
      7          21    D5     16
      8          22    D6     15
      9           9    ^WR     6
      10          7    A0      8
      11          8    A1      7
      12         14    ^BL    18
      GND        12    GND    10
      5V          6    VCC     9
      RST         5    ^CLR    3

*/

#include <DotMatrix.h>

byte  a[2] = {10, 11};     // Adress
byte  d[7] = {2, 3, 4, 5, 6, 7, 8}; // Data
byte  bl = 12;        // Blanking pin
byte  wr = 9;        // Write pin
byte  i;

void setup()
{
  Serial.begin(9600);
  pinMode(wr, OUTPUT);
  digitalWrite(wr, 1);
  pinMode(bl, OUTPUT);
  digitalWrite(bl, 1);
  pinMode(a[0], OUTPUT);
  pinMode(a[1], OUTPUT);

  for (i = 0; i < 7; i++) {
    pinMode(d[i], OUTPUT);
  }
}

void loop()
{
  writeWord("Test");
  delay(500);
  /*wrt(2, 0x65);
    delay(500);
    wrt(1, 0x73);
    delay(500);
    wrt(0, 0x74);
    delay(500); */

  for (i = 0; i < 5; i++) {
    digitalWrite(bl, 0);
    delay(500);
    digitalWrite(bl, 1);
    delay(500);
  }

}

void writeWord(String character)
{
  digitalWrite(a[0], 1);
  digitalWrite(a[1], 1);
  Serial.println(character[0]);
  for (int i = 0; i < 7; i++) {
    digitalWrite(d[i], bitRead(character[0], i));
  }
  wrt();
  
  digitalWrite(a[0], 0);
  digitalWrite(a[1], 1);

  for (int i = 0; i < 7; i++) {
    digitalWrite(d[i], bitRead(character[1], i));
  }
  wrt();

  digitalWrite(a[0], 1);
  digitalWrite(a[1], 0);

  for (int i = 0; i < 7; i++) {
    digitalWrite(d[i], bitRead(character[2], i));
  }
  wrt();

  digitalWrite(a[0], 0);
  digitalWrite(a[1], 0);

  for (int i = 0; i < 7; i++) {
    digitalWrite(d[i], bitRead(character[3], i));
  }
  wrt();
}

void wrt() {
  digitalWrite(wr, 0);
  delay(5);
  digitalWrite(wr, 1);
}
