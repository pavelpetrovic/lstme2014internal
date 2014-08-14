/*
 * display.c
 *
 * demonstrates how to use the display on z-shield
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "74hc595.h"

#define setLED1			PORTB |= (1 << 0)
#define clearLED1		PORTB &= ~(1 << 0)

int main(void)
{
	disp_init();
	DDRB |= 1;
	
    while(1)
    {
		for (uint8_t i = 0; i < DISP_NUM_REGS; i++)
			for (uint8_t j = 0; j < 8; j++)
			{
				disp_set_pin(i, j, 1);
				disp_write();
				if (j & 1) clearLED1;
				else setLED1;
				_delay_ms(750);
				disp_set_pin(i, j, 0);
				disp_write();
			} 
    }
	
	return 0;
}