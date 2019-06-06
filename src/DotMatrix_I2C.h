#ifndef DotMatrix_I2C_h
#define DotMatrix_I2C_h

#include <inttypes.h>
#include "Print.h"

class DotMatrix_I2C 
{
	public:
		DotMatrix_I2C(uint8_t digits, uint8_t clr, uint8_t wr, uint8_t bl, uint8_t ad0, uint8_t ad1, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6);
		void 			
			init(uint8_t digits, uint8_t clr, uint8_t wr, uint8_t bl, uint8_t ad0, uint8_t ad1, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6),
			clear(),
			blink(int n, int ms),
			noblink(),
			write(int out),
			writeWord(uint8_t displays, int pos, String character),
			writeChar(uint8_t pos, char character), 
			loadChar(char character),
			shiftRight(uint8_t displays, String character, int times, int dely),
			shiftLeft(uint8_t displays, String character, int times, int dely);
		String
			addBlanks(uint8_t displays, String character);
		uint8_t
			digits,
			character;
	private:
		uint8_t 
			_ce1_pin,
			_ce2_pin,
			_clr_pin,
			_wr_pin,
			_bl_pin,
			address_pins[2],
			data_pins[7];
};

#endif