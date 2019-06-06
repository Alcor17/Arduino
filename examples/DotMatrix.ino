/*

   code for OSRAM DLx2416 or DLx3416


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

const uint8_t DISPLAYS = 1; //number of 5x7 dots displays

byte  a[2] = {10, 11};     // Adress A0 and A1 of the DLx2416
byte  d[7] = {2, 3, 4, 5, 6, 7, 8}; // Data
byte  bl = 12;        // Blanking pin
byte  wr = 9;        // Write pin
byte  i;

// Initialize the display using DotMatrix library
DotMatrix dm(DISPLAYS, clr, wr, bl, ad0, ad1, d0, d1, d2, d3, d4, d5, d6);

void setup()
{
  pinMode(wr, OUTPUT);
  digitalWrite(wr, 1);
  pinMode(bl, OUTPUT);
  digitalWrite(bl, 1);
  pinMode(a[0], OUTPUT);
  pinMode(a[1], OUTPUT);

  for (i = 0; i < 7; i++) {
    pinMode(d[i], OUTPUT);
  }
  //Not blink the display
  dm.noblink();
}

void loop()
{
  //Delete the display memory 
  dm.clear();
 //write in 1 Displays, beginning at 0 position or MSB the string "Test"
  dm.writeWord(DISPLAYS, 0, "Test");
  delay(500);

  //Blink the display 5 times with 200ms delay
  dm.blink(5, 200);

}
