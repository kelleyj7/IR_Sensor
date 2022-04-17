#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "libLCD/hd44780.h"
#include <avr/interrupt.h>
#include "USART.h"
#include <avr/pgmspace.h>
#include <math.h>




#define irIn	(1<<PD2)

void test_blink();
void test_ON();
void test_Off();
void test_Flash();
void my_delay_ms(unsigned int delay);
void timer_setup();
void setup();


volatile char pulseHigh;
volatile int timerCount;
//uint16_t arr[36];
int arr[36];
volatile  int count;
char flag = 0;
ISR(TIMER1_COMPA_vect)
{


	timerCount++;



}


ISR(PCINT2_vect) {


	if ((PIND & irIn) < 1) {
		int counter = timerCount;
		timerCount = 0;


		if (count < 35) {
			if (counter > 14) {
				arr[count] = 5;
					transmitByte('5');
				count++;
			}

			else if (counter > 2) {
					transmitByte('1');
				arr[count] = 1;
				count++;
			}
			else if (counter <= 2) {
				transmitByte('0');
				arr[count] = 0;
				count++;
			}
		}
	}


	//transmitByte('0');


}





int main(void) {
	count = 0;
	setup();

	timer_setup();
	
	PCMSK2 |= _BV(PCINT18);
	PCICR |= _BV(PCIE2);
	EICRA |= (1 << ISC11);
	EICRA |= (1 << ISC10);
	sei();
	//int arr2[36] = { 5,5,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,5,5 };
	while (1) {
		if (count > 34) {
			int k = 13;
			int code = 0;
			transmitByte('\n');
			for (int i =20; i<34;i++){
				
				transmitByte('0'+(arr[i]%10));
				if (arr[i]== 1) {
					code += pow(2, k);
				}
				k--;
			}
			transmitByte('\n');
			printWord(code+1);
			transmitByte('\n');
			if (code == 12494) {
				test_ON();
			}
			else if (code == 6373) {
				test_Off();
			}
			else {
				transmitByte('n');
		
				
			}
			my_delay_ms(500);
			count = 0;
			for (int i = 0; i < 36; i++) {
				arr[i] = 0;
			}
			timerCount = 0;
		}



	
	}



}

void setup() {
	initUSART();
//	LCD_Setup();
//	LCD_Clear();

}



void timer_setup() {
	cli(); // stop interrupts
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1 = 0; // initialize counter value to 0
			   // set compare match register for 1777.1853826502277 Hz increments
	OCR1A = 9002; // = 16000000 / (1 * 1777.1853826502277) - 1 (must be <65536)
				  // turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 1 prescaler
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	/*
	timerCount = 0;

	TCCR1A = (1 << WGM12);//ctc mode
	OCR1A = 0x08;

	TIMSK1 |= (1 << OCIE1A);



	TCCR1B = (1 << CS10);

	//TCCR0B = (1 << CS01);  //prescalling set to 1/64
	//TIMSK1 = (1 << TOIE1);
	*/
}



void test_blink() {
	DDRB |= _BV(DDB5);

	/*while(1){


	PORTB ^= (1 << PORTB5);
	my_delay_ms(num);

	}*/
	while (1) {
		PORTB |= _BV(PORTB5);

		my_delay_ms(500);

		PORTB &= ~_BV(PORTB5);

		my_delay_ms(500);

	}
}
void test_ON() {
	DDRB |= _BV(DDB4);
	PORTB |= _BV(PORTB4);

}
void test_Off() {
	DDRB |= _BV(DDB4);
	PORTB &= ~_BV(PORTB4);
}
void test_Flash() {
	DDRB |= _BV(DDB5);
	PORTB |= _BV(PORTB5);
	my_delay_ms(1000);
	PORTB &= ~_BV(PORTB5);
	//my_delay_ms(1);

}


void my_delay_ms(unsigned int delay)
{
	unsigned int i;

	for (i = 0; i<(delay / 10); i++)
	{
		_delay_ms(10);
	}
	for (i = 0; i < (delay % 10); i++)
	{
		_delay_ms(1);
	}
}













/*


while (1) {

if (arr[38] == 1&&arr[48]!=1) {
DDRB |= _BV(DDB3);
PORTB |= _BV(PORTB3);
count = 0;
}
else if (arr[48] == 1 && arr[52] != 1) {
DDRB |= _BV(DDB4);
PORTB |= _BV(PORTB4);
count = 0;
}
else if (arr[52] == 1) {
DDRB |= _BV(DDB5);
PORTB |= _BV(PORTB5);
count = 0;
}








}























C:\Users\User\Documents\387_Homework\avr-gcc-11.1.0\bin\avr-gcc.exe -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o IR_Sensor.o IR_Sensor.c; C:\Users\User\Documents\387_Homework\avr-gcc-11.1.0\bin\avr-gcc.exe -mmcu=atmega328p IR_Sensor.o -o IR_Sensor;
C:\Users\User\Documents\387_Homework\avr-gcc-11.1.0\bin\avr-objcopy.exe -O ihex -R .eeprom IR_Sensor IR_Sensor.hex; C:\Users\User\Documents\387_Homework\avr-gcc-11.1.0\bin\avrdude.exe -patmega328p -Pcom3 -carduino -D -U flash:w:IR_Sensor.hex:i










if (!(arr[0] == 0)) {
test_ON();
}
if (!(arr[1] == 1)) {
test_ON();
}
if (!(arr[2] == 0)) {
test_ON();
}
if (!(arr[3] == 1)) {
test_ON();
}
if (!(arr[4] == 0)) {
test_ON();
}if (!(arr[5] == 1)) {
test_ON();
}

if (!(arr[6] == 0)) {
test_ON();
}
if (!(arr[7] == 1)) {
test_ON();
}
if (!(arr[8] == 1)) {
test_ON();
}
if (!(arr[9] == 1)) {
test_ON();
}
if (!(arr[10] == 1)) {
test_ON();
}if (!(arr[11] == 1)) {
test_ON();
}
if (!(arr[12] == 0)) {
test_ON();
}
if (!(arr[12] == 0)) {
test_ON();
}







ISR(TIMER0_COMPA_vect)
{
if (count == 9) {
TCCR1B &= ~(1 << CS12);
TCCR1B &= ~(1 << CS11);
TCCR1B &= ~(1 << CS10);

for (int i = 0; i < 9; i++)
{
char oneLetter = pgm_read_byte(&(arr[i]));
transmitByte(oneLetter);
}
}
timerCount++;
if (1<PIND & irIn) {
arr[count] = 1;
count++;
flag = 1;
}
else if (flag == 1) {
arr[count] = 0;
count++;
}

if (timerCount == 20) { //testing turns off interupt

}









if ((PINB & irIn)<1) {
arr[count] = 1;
count++;
flag = 1;
}
else if (flag == 1) {
arr[count] = 0;
count++;
}


TCCR1B &= ~(1 << CS12);
TCCR1B &= ~(1 << CS11);
TCCR1B &= ~(1 << CS10);




*/
