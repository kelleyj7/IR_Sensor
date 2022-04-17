#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "IO_Macros.h"

#include <avr/pgmspace.h>
#include "USART.h"

// "Make: AVR Programming" by Elliot Williams

const char myVeryLongString[] PROGMEM = "\r\nHi there, \
this is an example of a long string.\r\n\
The kind that you wouldnot want to store in RA.\r\n";
const uint16_t sixteenBits PROGMEM = 12345;
const char otherVeryLongString[] = "\r\nHi there, \
this is an example of a long string.\r\n\
The kind that you wouldnot want to store in RAM.\r\n";
const uint16_t othersixteenBits = 12345;

/* PROTOTPYES */
void my_delay_ms(unsigned int delay);

int main()
{
	char oneLetter;
	uint8_t i;

	/* LCD init */
//	LCD_Setup();
	initUSART();

	while (1) 
	{
#if 1
		for (i = 0; i < sizeof(myVeryLongString); i++) 
		{
			oneLetter = pgm_read_byte(&(myVeryLongString[i]));
			transmitByte(oneLetter);
			my_delay_ms(100); /* slow it down to simulate typing effect :) */
		}
		my_delay_ms(1000);
#endif

		printWord(&sixteenBits); /* this throws a compiler warning... */
		transmitByte('\r');
		transmitByte('\n');

		printWord(pgm_read_word(&sixteenBits));
		transmitByte('\r');
		transmitByte('\n');

		printWord(&othersixteenBits); 
		transmitByte('\r');
		transmitByte('\n');

		printWord(othersixteenBits);
		transmitByte('\r');
		transmitByte('\n');

		printWord(&myVeryLongString);
		transmitByte('\r');
		transmitByte('\n');

		printWord(&otherVeryLongString);
		transmitByte('\r');
		transmitByte('\n');
		transmitByte('\n');
		my_delay_ms(10000);
	} /* End event loop */
	return (0); /* This line is never reached */

	return 1;
}

/* 
 * Handles larger delays the _delay_ms can't do by itself (not sure how accurate)  
 * Note no DBC as this function is used in the DBC !!! 
 *
 * borrowed from : https://www.avrfreaks.net/forum/delayms-problem 
 * */
void my_delay_ms(unsigned int delay) 
{
	unsigned int i;

	for (i=0; i<(delay/10); i++) 
	{
		_delay_ms(10);
	}
	if (delay % 10) {
		_delay_ms(delay % 10);
	}
}
