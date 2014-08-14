/*
 * display4x8.c
 *
 * shows an example how to use the 4-digit 7-segment display, based on software i2c implementation
 */ 

// 4-digit Display, connect:
// CLK -> D2
// DIO -> D3
// or change defines in four_digit_display.h

#include <avr/io.h>
#include <stdio.h>
#include "four_digit_display.h"
#include "wait.h"

#include "terminal.h"

int main(void)
{
	init_terminal();
	printf("display4x8");
	four_digit_init();
	printf("...initialized...");
    while(1)
    {
		four_digit_set_brightness(4);
		// run 0-f from right to left
		uint8_t pos1 = 3;
		uint8_t num1 = 0;
		uint8_t num2 = 0;
		uint8_t num;
		printf("0");
		while (num1 < 0x10)
		{	
			for (num = num1; num <= num2; num++)
				four_digit_display(pos1 + (num - num1), num);
			
			uint8_t posC = pos1 + (num - num1);
			while (posC < 4) four_digit_display(posC++, 0x7f);
							
			if (pos1 > 0) pos1--;
			else num1++;
			num2++;
			if (num2 > 0xf) num2--;
			else printf("%x", num2);
			
			wait(250);
		}
		four_digit_clear();
		
		// flash middle column 3-times
		for (int i = 0; i < 3; i++)
		{
			four_digit_set_point(POINT_ON);
			four_digit_display(1, 0x7f);
			wait(300);
			four_digit_set_point(POINT_OFF);
			four_digit_display(1, 0x7f);
			wait(300);
		}
		
		// show brightness levels
		for (int b = 0; b <= BRIGHTEST; b++)
		{
			four_digit_set_brightness(b);
			for (int i = 0; i < 4; i++)
				four_digit_display(i, 8);
			wait(250);
		}
		
		four_digit_clear();
	}
}