/*
 * 74hc595.c
 *
 * controls the 4 chained shift-registers that drive the 7-segment display
 */ 

#include "74hc595.h"

static uint8_t regs[DISP_NUM_REGS];
	
void disp_init()
{
	DISP_DDR1 |= DISP_DATA | DISP_LATCH | DISP_CLK;
	DISP_DDR2 |= DISP_OE;
	DISP_PORT1 &= ~(DISP_LATCH | DISP_CLK);
	disp_clear();
	disp_write();
	DISP_PORT2 &= ~DISP_OE;
}

void disp_write()
{
	DISP_PORT1 &= ~DISP_CLK;
		
	//iterate through the registers
	for (int8_t i = DISP_NUM_REGS - 1; i >=  0; i--)
	{		
		//iterate through the bits in each registers
		uint8_t mask = 128;
		for (int8_t j = 7; j >=  0; j--)
		{
			DISP_PORT1 &= ~DISP_LATCH;
			DISP_PORT1 &= ~DISP_CLK;

			if (regs[i] & mask) DISP_PORT1 &= ~DISP_DATA;
			else DISP_PORT1 |= DISP_DATA;

			DISP_PORT1 |= DISP_LATCH;
			DISP_PORT1 |= DISP_CLK;
			mask >>= 1;				
		}
	}
	DISP_PORT1 |= DISP_CLK;
	DISP_PORT1 &= ~DISP_LATCH;
	DISP_PORT1 |= DISP_LATCH;
}
	
void disp_set_pin(uint8_t reg, uint8_t pin, uint8_t val)
{
	if (val) regs[reg] |= (1 << pin);
	else regs[reg] &= ~(1 << pin);
}

void disp_set_all(uint8_t val)
{
	for(uint8_t i = 0; i < DISP_NUM_REGS; i++)
		for (uint8_t j = 0; j < 8; j++)
			disp_set_pin(i, j, val);		
}

void disp_clear()
{
	disp_set_all(0);
}
