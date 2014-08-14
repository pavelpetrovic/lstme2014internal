/*
 * servo_timer_PWM.c
 *
 * simple servo control with timer1 with high resolution
 */ 

#include <avr/io.h>
#include "wait.h"

#define SERVO_MIN 1000
#define SERVO_MAX 4000

// this program is using OCR1A = PB1 = arduino digital pin 9

void init_servo_timer()
{
	DDRB = (5 << 1) | (1 << 1);
	
	//prescaler 8, mode 15 (fast PWM with ICR1), Clear OC1A/OC1B on Compare Match = mode 0b10, 
	// that is 2MHz frequency => 50Hz => TOP = 40000
	OCR1A = 2000;  // reasonable values around 1000-3000
	ICR1 = 39999U;
	TIMSK1 = 0;
	TCCR1A = 0b10000010;
	TCCR1B = 0b00011010;
}


int main(void)
{
	uint16_t i;
	
	init_servo_timer();
	
	for (i = 0; i < 3; i++)  // flash LED 3 times
	{
		PORTB |= (1 << 5);
		wait(300);
		PORTB &= ~(1 << 5);
		wait(300);
	}
	
    while(1)
    {
		
		for (i = (SERVO_MIN + SERVO_MAX) / 2; i < SERVO_MAX; i++)
		{
		  OCR1A = i;
		  wait(1);
		}
		
		PORTB |= (1 << 5);
		for (i = SERVO_MAX; i > SERVO_MIN; i--)
		{
			OCR1A = i;
			wait(1);
		}
		PORTB &= ~(1 << 5);
		for (i = SERVO_MIN; i < (SERVO_MIN + SERVO_MAX) / 2; i++)
		{
			OCR1A = i;
			wait(1);
		}		
    }
}
