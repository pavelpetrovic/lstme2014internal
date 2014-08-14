/*
 * buttons.c
 *
 * example of using LEDs and buttons on Zenit shield
 */ 


#include <avr/io.h>
#include <stdio.h>


#define BUT1	(0 != (PINC & (1 << 0)))
#define BUT2	(0 != (PIND & (1 << 0)))
#define BUT3	(0 != (PIND & (1 << 1)))
#define BUT4	(0 != (PIND & (1 << 4)))

#define setLED1			PORTB |= (1 << 0)
#define setLED2			PORTD |= (1 << 7)
#define setLED3			PORTD |= (1 << 6)
#define setLED4			PORTD |= (1 << 5)

#define clearLED1		  PORTB &= ~(1 << 0)
#define clearLED2		  PORTD &= ~(1 << 7)
#define clearLED3		  PORTD &= ~(1 << 6)
#define clearLED4		  PORTD &= ~(1 << 5)

void init_buttons()
{
		DDRC &= (1 << 0);  // button 1 on ADC0 (PC0)
		PORTC |= (1 << 0);  // internal pull-up on
		// buttons 2,3,4 on D0 D1 D4 (PD0,1,4)
		DDRD &= (1 << 0) | (1 << 1) | (1 << 4);
		PORTD |= (1 << 0) | (1 << 1) | (1 << 4);  // internal pull-ups on
}

void init_LEDs()
{
	DDRD |= (1 << 5) | (1 << 6) | (1 << 7);
	DDRB |= (1 << 0);
	PORTD &= ~((1 << 5) | (1 << 6) | (1 << 7));
	PORTB &= ~(1 << 0);
}

int main(void)
{
	init_buttons();
	init_LEDs();
	
    while(1)
    {
	    if (BUT1) setLED1; else clearLED1;
		if (BUT2) setLED2; else clearLED2;
		if (BUT3) setLED3; else clearLED3;
		if (BUT4) setLED4; else clearLED4;
    }
}
