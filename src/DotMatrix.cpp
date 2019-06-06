#include "DotMatrix.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

DotMatrix::DotMatrix(uint8_t digits, uint8_t clr, uint8_t wr, uint8_t bl, uint8_t ad0, uint8_t ad1, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6) {
	init(digits, clr, wr, bl, ad0, ad1, d0, d1, d2, d3, d4, d5, d6);
}

void DotMatrix::init(uint8_t digits, uint8_t clr, uint8_t wr, uint8_t bl, uint8_t ad0, uint8_t ad1, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6) {
	
	_clr_pin = clr;
	_wr_pin = wr;
	_bl_pin = bl;
	
	address_pins[0] = ad0;
	address_pins[1] = ad1;
  
	data_pins[0] = d0;
	data_pins[1] = d1;
	data_pins[2] = d2;
	data_pins[3] = d3; 
	data_pins[4] = d4;
	data_pins[5] = d5;
	data_pins[6] = d6;
	
	DDRC = B00000001; //for 1 display, 4 digits
	if (digits == 2) {
		DDRC = B00000011; //for 2 displays, 8 digits
	}
	else if (digits == 3) {
		DDRC = B00000111; //for 3 displays, 12 digits
	}
	else if (digits == 4) {
		DDRC = B00001111; //for 4 displays, 16 digits
	}
	pinMode(_clr_pin,OUTPUT);
	pinMode(_wr_pin,OUTPUT);
	pinMode(_bl_pin,OUTPUT);
	
	pinMode(address_pins[0],OUTPUT);
	pinMode(address_pins[1],OUTPUT);
	
	for (int j = 0; j < 7; j++) {
		pinMode(data_pins[j], OUTPUT);
	}
}

void DotMatrix::clear() {
	digitalWrite(_clr_pin, LOW);
	delay(10);
	digitalWrite(_clr_pin, HIGH);
}

void DotMatrix::blink(int n, int ms) {
	for (int i = 0; i < n; i++) {
		digitalWrite(_bl_pin, 0);
		delay(ms);
		digitalWrite(_bl_pin, 1);
		delay(ms);
	}
}

void DotMatrix::noblink() {
	digitalWrite(_bl_pin, 1);
}

void DotMatrix::write() {
	digitalWrite(_wr_pin, LOW);
	delay(5);
	digitalWrite(_wr_pin, HIGH);
}

void DotMatrix::writeWord(uint8_t displays, int pos, String character) {
String c, s, blanks;

	blanks = character;
	
	for (int y = 0;y < pos;y++) {
		s += " ";
	}
	
	if (pos < 0) {
		for (int z = -(pos);z < blanks.length();z++) {
			s += blanks[z];
		}
		for (int y = s.length();y < blanks.length();y++) {
			s += ' ';
		}
	}
	else {		
		for (int z = pos;z < blanks.length();z++) {
			s += blanks[z - pos];
		}
	}
	
	for (int x = displays;x > 0;x--) {
		switch (x) {
			case 1:	
				PORTC = B11111110;
				for (int n = 3;n >= 0;n--) {
					c += s[n];
				}
				break;
			case 2:	
				PORTC = B11111101;
				for (int n = 7;n >= 4;n--) {
					c += s[n];
				}
				break;
			case 3:		
				PORTC = B11111011;
				for (int n = 11;n >= 8;n--) {
					c += s[n];
				}
				break;
			case 4:	
				PORTC = B11110111;
				for (int n = 15;n >= 12;n--) {
					c += s[n];
				}
				break;
		}
	
		for (int k = 3;k >= 0;k--) {
			digitalWrite(address_pins[0], bitRead(k, 0));
			digitalWrite(address_pins[1], bitRead(k, 1));			
	
			loadChar(c[k]);
		}
		c = "";
	}
}

void DotMatrix::writeChar(uint8_t pos, char character) {
int nm = 3 - (pos % 4);

	if (pos >= 12 && pos < 16) {
		PORTC = B11110111;
	}
	else if (pos >= 8 && pos < 12) {		
		PORTC = B11111011;
	}
	else if (pos >= 4 && pos < 8) {		
		PORTC = B11111101;
	}
	else if (pos < 4) {	
		PORTC = B11111110;
	}
	digitalWrite(address_pins[0], bitRead(nm, 0));
	digitalWrite(address_pins[1], bitRead(nm, 1));
	
	loadChar(character);
}

void DotMatrix::loadChar(char character) {
	for (int i = 0; i < 7; i++) {
		digitalWrite(data_pins[i], bitRead(character, i));
	}
	write();
}

void DotMatrix::shiftRight(uint8_t displays, String character, int times, int dely) {
int n;
char c;
int cons = displays * 4;Serial.println(character);
	for (int k = 0;k < times;k++){
		for (int j = 0;j <= cons;j++) {
			writeWord(displays, j, character);
			delay(dely);		
		}
		n = 0;
		while (n < cons) {
			for (int v = 0;v <= n;v++) {
				writeChar(v, character[(cons - 1) - n + v]);
			}
			delay(dely);
			n++;
		}		
	}	
}

void DotMatrix::shiftLeft(uint8_t displays, String character, int times, int dely) {
int n;
char c;
int cons = displays * 4;
	for (int k = 0;k < times;k++){
		for (int j = 0;j <= cons;j++) {
			writeWord(displays, -j, character);
			delay(dely);		
		}
		n = cons - 1;
		while (n >= 0) {
			for (int v = 0;v <= (cons - 1) - n;v++) {
				writeChar((cons - 1) - v, character[(cons - 1) - n - v]);
			}
			delay(dely);
			n--;
		}	
	}
}

String DotMatrix::addBlanks(uint8_t displays, String character) {
	for (int j = 0;j < displays * 4;j++){
		if (character[j] == char(0)) character += " ";
	}
	return character;
}