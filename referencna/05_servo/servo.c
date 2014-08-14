/*
 * servo.c
 *
 * simple servo control with delay in loop
 */ 

#include <avr/io.h>
#include "wait.h"

#define SERVO_DDR		DDRB
#define SERVO_PORT		PORTB
#define SERVO_SIGNAL	(1 << 1)

void send_pulse(uint16_t usec)
{
	SERVO_PORT |= SERVO_SIGNAL;
	_delay_us(usec);
	SERVO_PORT &= ~SERVO_SIGNAL;
}

int main(void)
{
	uint16_t i;
	
	SERVO_DDR |= SERVO_SIGNAL;
	DDRB |= (1 << 5);  // LED
	
    while(1)
    {
		PORTB |= (1 << 5);
		for (i = 0; i < 1000; i += 20)		
		{
			send_pulse(1000 + i);
			wait(20);
		}
			
		PORTB &= ~(1 << 5);
		for (i = 1000; i > 0; i -= 20)
		{
			send_pulse(1000 + i);
			wait(20);
		}
    }
}
