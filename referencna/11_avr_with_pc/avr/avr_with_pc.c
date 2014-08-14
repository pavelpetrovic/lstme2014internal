/*
 * avr_with_pc.c
 *
 * example of communication with PC over serial port
 * communication is initiated with ! from PC answered with @ from AVR
 * receives strings separated by ^ from PC and sends reversed strings back to PC
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "terminal.h"

int main(void)
{	
	char buf[100];
	int p;
	char c;
	
	init_terminal();
	
	do { scanf("%c", &c); } while (c != '!');
	printf("@");
		
	while(1)
    {
		p = 0;
		do { scanf("%c", buf + p); p++; } while (buf[p - 1] != '^');
		
		p--;
		buf[p] = 0;
		uint8_t n = strlen(buf);
		
		for (int i = 0; i < n / 2; i++)
		{			
			char c = buf[i];
			buf[i] = buf[n - i - 1];
			buf[n - i - 1] = c;
		}
		
		printf("%s^", buf);
    }
}
