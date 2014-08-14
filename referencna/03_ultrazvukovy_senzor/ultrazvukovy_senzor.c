/*
 * UltrazvukovySenzor.c
 *
 * measuring distances with ultrasonic sensor
 * with measuring pulse directly within a loop
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "terminal.h"
#include "wait.h"

// trigger at PD6 (pin 6), echo at PD7 (pin 7)

#define ULTRASONIC_PORT		PORTD
#define ULTRASONIC_PIN		PIND
#define ULTRASONIC_DDR		DDRD
#define ULTRASONIC_TRIGGER	6
#define ULTRASONIC_ECHO		7

int main(void)
{
	uint16_t distance;
	
	DDRB |= (1 << 5); // LED
	
	ULTRASONIC_DDR |= (1 << ULTRASONIC_TRIGGER);
	ULTRASONIC_DDR &= ~(1 << ULTRASONIC_ECHO);
	ULTRASONIC_PORT |= (1 << ULTRASONIC_ECHO);  // internal pull-up on echo
	
	init_terminal();
	
    while(1)
    {
		distance = 0;
		
		PORTB |= (1 << 5);  // LED on
		ULTRASONIC_PORT |= (1 << ULTRASONIC_TRIGGER);
		_delay_us(10);
		ULTRASONIC_PORT &= ~(1 << ULTRASONIC_TRIGGER);
				
		while ((ULTRASONIC_PIN & (1 << ULTRASONIC_ECHO)) == 0)
		{
		  _delay_us(10); 
		  if (distance++ > 2000) break;  // in case we loose connection or something goes wrong
		}
		
		distance = 0;		
		while (ULTRASONIC_PIN & (1 << ULTRASONIC_ECHO))
		{
			_delay_us(28);
			distance++;
			if (distance > 1000) break;  // in case we loose connection or something goes wrong
		}
		PORTB &= ~(1 << 5);  // LED off
		
		printf("d=%d\n\r", distance);
		wait(300);
    }
}
