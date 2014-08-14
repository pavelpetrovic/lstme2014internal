/*
 * Joystick_ADC.c
 *
 * demo program to show the joystick state
 */ 


#include <avr/io.h>
#include <stdio.h>

#include "wait.h"
#include "terminal.h"

//setup ADC ports here (i.e. 0 for ADC0, 1 for ADC1, etc.)
#define JOY_X	0
#define JOY_Y	1

#define JOY_BUTTON_DDR	DDRD
#define JOY_BUTTON_PORT PORTD
#define JOY_BUTTON_PIN	PIND
#define JOY_BUTTON_IN	(1 << 6)


int main(void)
{
	init_terminal();
	printf("Hello, do you have joystick connected? (A0,A1,D6)\n\r");
	
	JOY_BUTTON_DDR &= ~JOY_BUTTON_IN; // all inputs
	PORTD = JOY_BUTTON_IN; // setup internal pull-up on joystick button
	
	//ref. voltage: AVCC with external capacitor at AREF pin, select ADC0 x channel
	//adc on, no interrupt, no trigger, prescaler 128
	ADCSRA = 0b10000111;
	ADCSRB = 0;
	//disable digital input pin register on all analog inputs a0-a6
	DIDR0 = 0b00111111;
		
    while(1)
    { 
		ADMUX = 0b01000000 + JOY_X; //select X channel
		ADCSRA |= (1 << ADSC); // start conversion
		while (0 == (ADCSRA & (1 << ADIF))); // wait for the conversion to be ready
		uint16_t x = ADC;
		
		ADMUX = 0b01000000 + JOY_Y; //select y channel
		ADCSRA |= (1 << ADSC); // start conversion
		while (0 == (ADCSRA & (1 << ADIF))); // wait for the conversion to be ready
		uint16_t y = ADC;
		uint8_t but = (0 != (JOY_BUTTON_PIN & JOY_BUTTON_IN));
		
		printf("%u %u %c\r\n", x, y, but + '0');
        wait(300); 
    }
}
