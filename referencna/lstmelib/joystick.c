/*
 * joystick.c
 *
 */ 


#include <avr/io.h>
#include "joystick.h"

void joystick_init()
{	
	//ref. voltage: AVCC with external capacitor at AREF pin, select ADC0 x channel
	//adc on, no interrupt, no trigger, prescaler 128
	ADCSRA = 0b10000111;
	ADCSRB = 0;
	//disable digital input pin register on all analog inputs a0-a6
	DIDR0 = 0b00111111;
	JOY_BUTTON_DDR &= ~JOY_BUTTON_IN; // all inputs
	PORTD = JOY_BUTTON_IN; // setup internal pull-up on joystick button
}

uint16_t joystick_x()
{		
 		ADMUX = 0b01000000 + JOY_X; //select X channel
		ADCSRA |= (1 << ADSC); // start conversion
		while (0 == (ADCSRA & (1 << ADIF))); // wait for the conversion to be ready
		return ADC;
}

uint16_t joystick_y()
{
		ADMUX = 0b01000000 + JOY_Y; //select y channel
		ADCSRA |= (1 << ADSC); // start conversion
		while (0 == (ADCSRA & (1 << ADIF))); // wait for the conversion to be ready
		return ADC;
}

uint8_t joystick_button()
{
		return (0 != (JOY_BUTTON_PIN & JOY_BUTTON_IN));
}		
