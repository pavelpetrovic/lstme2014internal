/*
 * shared/terminal.c
 *
 * configures serial port 0 (USART0) as standard input/output
 * with config parameters 76800-8-N-1  
 */ 

#include <avr/io.h>
#include <stdio.h>

uint8_t terminal_echo_on = 1;

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
	//echo on/off
	if (terminal_echo_on)
		usart0_putchar((char)c, 0);
	return c;
}

FILE usart0 = FDEV_SETUP_STREAM(usart0_putchar, usart0_getchar, _FDEV_SETUP_RW);

void init_terminal()
{
	// USART0: enable Rx, Tx, no interrupts, 8-N-1, 76.8 kbps
	UCSR0A = 0;
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
	UBRR0 = 12; //76800

	stdout = &usart0;
	stdin = &usart0;	
}

// returns 1, if a new char is ready to be read, or 0, otherwise
uint8_t char_ready()
{
	return (UCSR0A & RXC0) >> RXC0;  
}
