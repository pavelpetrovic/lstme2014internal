/*
 * sound.c
 *
 * demonstrates using the piezo speaker on z-shield
 */ 

#define __DELAY_BACKWARD_COMPATIBLE__ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	uint16_t d[3] = { 2273, 1136, 568 };
	DDRB |= 2;
	
    while(1)
    {
		for (int j = 0; j < 3; j++)
			for (int i = 0; i < 500 / (4 >> j); i++)
			{
				PORTB |= 2;
				_delay_us(d[j]);
				PORTB &= ~2;
				_delay_us(d[j]);
			}
	}
}