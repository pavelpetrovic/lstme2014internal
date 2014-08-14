/*
 * Blika1LED.c
 *  pravidelne blika LED na doske
 *  cakacia funkcia _delay_ms 
 */ 

#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>

void wait(uint16_t ms)
{
	while (ms > 16)
	{
		_delay_ms(16);
		ms -= 16;
	}
	_delay_ms(ms);
}

int main(void)
{
	DDRB = (1 << 5);  // 32;
    while(1)
    {
        PORTB = (1 << 5);
		wait(500);
		PORTB = 0;
		wait(500); 
    }
}
