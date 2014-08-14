/*
 * blika_LED_timer.c
 *
 * using timer to measure time interval precisely
 */ 

// blika LED na doske s periodou zadanou cez konzolu,
// zaroven ukazuje priklad, kde je printf a scanf s float-mi 
// t.j. -Wl,-u,vfprintf -Wl,-u,vfscanf -lprintf_flt -lscanf_flt

#include <avr/io.h>
#include <stdio.h>
#include "terminal.h"

void init_timer(uint16_t timer_max)
{
	// timer 1, prescaler 1024, CTC mode 4, TOP is OCR1A, OCxx pins disconnected
	OCR1A = timer_max - 1;
	TIMSK1 = 0;
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001101;	
	TCNT1 = 0;
}

int main(void)
{
	float pol_periody;
	
	DDRB = 32;  // LED on PB5
	
	init_terminal();
	printf("\n\r---\n\rZadaj polovicu periody blikania LED v sekundach\n\rcize cislo 0..4.194304 sec.\n\rpresnost je 1/15625-tina sec, cize 0.000064 sec.\n\rpol periody: ");
	scanf("%f", &pol_periody);
	if (pol_periody > 4.194394) pol_periody = 4.194394;
	if (pol_periody < 0.0) pol_periody = 0.0;
	
	uint16_t delay = (uint16_t)(pol_periody * (float)15625.0);
	printf("\n\rpol periody %f s, cize %u pulzov casovaca\n\r", pol_periody, delay);
	
	init_timer(delay);
	
    while(1)
	{
	   if (TIFR1 & (1 << OCF1A))      // timer1 overflow?
	   {
	     PORTB ^= (1 << 5);                 // invert LED
		 TIFR1 |= (1 << OCF1A);       // clear overflow flag
	   }
	}
}
