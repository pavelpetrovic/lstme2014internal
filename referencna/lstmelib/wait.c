/*
 * wait.c
 *
 *  utility function to wait given number of milliseconds
 */ 
 
#include "wait.h"

void wait(uint16_t ms)
{
	while (ms > 16)
	{
		_delay_ms(16);
		ms -= 16;
	}
	_delay_ms(ms);
}
