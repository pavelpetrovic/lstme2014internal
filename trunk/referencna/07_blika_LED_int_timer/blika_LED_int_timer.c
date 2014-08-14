/*
 * blika_LED_int_timer.c
 *
 * using 16-bit timer1 for periodic events (flashing LED)
 */ 

// blika LED na doske v preruseni casovaca s periodou zadanou cez konzolu

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "terminal.h"

void init_timer(uint16_t timer_max)
{
	// timer 1, prescaler 1024, CTC mode 4, TOP is OCR1A, OCxx pins disconnected, 
	// enable interrupt on Timer/Counter1 Compare Match A
	OCR1A = timer_max - 1;
	TIMSK1 = (1 << OCIE1A);
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001101;	
	TCNT1 = 0;
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << 5); 	
}

int main(void)
{
	uint32_t pol_periody;
	DDRB = (1 << 5);  // LED on PB5
	
	init_terminal();
	printf("\n\r---\n\rZadaj polovicu periody blikania LED v microsekundach,\n\rcize cislo 0..4194304 usec.\n\rPresnost je 1/15625-tina sec, cize 64 usec: ");
	scanf("%lu", &pol_periody);
	uint16_t delay = (uint16_t)(pol_periody / 64);
	printf("\n\rpol periody %lu usec, cize %u pulzov casovaca\n\r", pol_periody, delay);
	
	init_timer(delay);
	
	while(1);
}
