/*
 * Konzola.c
 *  setup serial communication with PC
 *  blocking read/write
 */ 

#include <avr/io.h>
#include <stdio.h>

static uint8_t echo_on = 1;

int usart0_putchar(char c, FILE *stream __attribute__ ((unused))) // wait until the port is ready and then send the next char
{
	while (0 == (UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return 0;
}

int usart0_getchar(FILE *stream __attribute__ ((unused))) 	//wait for the char to be received and then return it
{
	while (0 == (UCSR0A & (1 << RXC0)));
	uint8_t c = UDR0;
	if (echo_on)
		usart0_putchar((char)c, 0);
	return c;
}

FILE usart0 = FDEV_SETUP_STREAM(usart0_putchar, usart0_getchar, _FDEV_SETUP_RW);

int main(void)
{
//	char guess_str[20];
	char his_name[21];
	int16_t guess;
	
	// USART0: enable Rx, Tx, no interrupts, 8-N-1, 76.8 kbps
	UCSR0A = 0;				
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
	UBRR0 = 12;
	
	stdout = &usart0;
	stdin = &usart0;
	
	while (1)
	{
		printf("Hello, my name may not be pronounced (You-Know-Who).\r\n");
		printf("What's your name? ");
		scanf("%s&n", his_name);												
		printf("\n\rNice to meet you, %s\n\r", his_name);
		guess = 0;
	
		while(1)
		{
    		printf("What's your guess [1-100]? ");
			if (0 == scanf("%d&n", &guess)) scanf("%*s&n");
			if (guess == 42) break;
			printf("\n\r%d? Nope. Try again.\r\n", guess);
		}
		
		printf("\n\rThat's not fair, you knew before.\r\n---\r\n");
	}
	
	return 0;
}