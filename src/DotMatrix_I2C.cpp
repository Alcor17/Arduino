#include "DotMatrix_I2C.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

DotMatrix_I2C::DotMatrix_I2C(uint8_t digits, uint8_t clr, uint8_t wr, uint8_t bl, uint8_t ad0, uint8_t ad1, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6) {
	init(digits, clr, wr, bl, ad0, ad1, d0, d1, d2, d3, d4, d5, d6);
}

void DotMatrix_I2C::init(uint8_t digits, uint8_t clr, uint8_t wr, uint8_t bl, uint8_t ad0, uint8_t ad1, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6) {
	
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
	
	pinMode(_clr_pin,OUTPUT);
	pinMode(_wr_pin,OUTPUT);
	pinMode(_bl_pin,OUTPUT);
	
	pinMode(address_pins[0],OUTPUT);
	pinMode(address_pins[1],OUTPUT);
	
	for (int j = 0; j < 7; j++) {
		pinMode(data_pins[j], OUTPUT);
	}
}

void DotMatrix_I2C::clear() {
	mcp.digitalWrite(_clr_pin, LOW);
	delay(10);
	mcp.digitalWrite(_clr_pin, HIGH);
}

void DotMatrix_I2C::blink(int n, int ms) {
	for (int i = 0; i < n; i++) {
		mcp.digitalWrite(_bl_pin, 0);
		delay(ms);
		mcp.digitalWrite(_bl_pin, 1);
		delay(ms);
	}
}

void DotMatrix_I2C::noblink() {
	mcp.digitalWrite(_bl_pin, 1);
}

void DotMatrix_I2C::write(int out) {
	mcp.writeGPIOAB(out);
	mcp.digitalWrite(_wr_pin, LOW);
	delay(10);
	mcp.digitalWrite(_wr_pin, HIGH);
}

void DotMatrix_I2C::writeWord(uint8_t displays, int pos, String character) {
String c, s, blanks;
uint16_t ces, _ces, adds, add1, out, t = 0xFF;

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
				ces = 0xFEFF;
				for (int n = 3;n >= 0;n--) {
					c += s[n];
				}
				break;
			case 2:
				ces = 0xFDFF;
				for (int n = 7;n >= 4;n--) {
					c += s[n];
				}
				break;
			case 3:
				ces = 0xFBFF;
				for (int n = 11;n >= 8;n--) {
					c += s[n];
				}
				break;
			case 4:
				ces = 0xF7FF;
				for (int n = 15;n >= 12;n--) {
					c += s[n];
				}
				break;
		}
	
		for (int k = 3;k >= 0;k--) {
			switch (k) {
				case 3: adds = 0xFFFF;
						break;
				case 2: adds = 0xEFFF;
						break;
				case 1: adds = 0xDFFF;
						break;
				case 0:	adds = 0xCFFF;
						break;
			}

			_ces = ces & adds;
			t = _ces >> 8;
			out = c[k] << 8; 
			out = out | t;
			write(out);
		}
		c = "";
	}
}

void DotMatrix_I2C::writeChar(uint8_t pos, char character) {
int nm = 3 - (pos % 4);
int ces, _ces, out = 0xFF;
	out = character << 8;
	if (pos >= 12 && pos < 16) {		
		ces = 0x00F7;
	}
	else if (pos >= 8 && pos < 12) {
		ces = 0x00FB;
	}
	else if (pos >= 4 && pos < 8) {	
		ces = 0x00FD;
	}
	else if (pos < 4) {		
		ces = 0x00FE;
	}
	out = out | ces;
	bitWrite(out, address_pins[0], bitRead(nm, 0));
	bitWrite(out, address_pins[1], bitRead(nm, 1));Serial.println(out,BIN);
	write(out);
}

void DotMatrix_I2C::shiftRight(uint8_t displays, String character, int times, int dely) {
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

void DotMatrix_I2C::shiftLeft(uint8_t displays, String character, int times, int dely) {
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

String DotMatrix_I2C::addBlanks(uint8_t displays, String character) {
	for (int j = 0;j < displays * 4;j++){
		if (character[j] == char(0)) character += " ";
	}
	return character;
}