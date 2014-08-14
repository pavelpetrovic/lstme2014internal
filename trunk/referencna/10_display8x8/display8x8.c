/*
 * display8x8.c
 *
 * shows how to use the 8x8 display on SPI (sw or hw)
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "wait.h"
#include "terminal.h"

// change if you like for non-hw version
#define DISP8x8_DDR	 DDRB
#define DISP8x8_PORT PORTB

//the following must be 4,32,8 for hw SPI on Arduino UNO  
//i.e. SS, SCK, MOSI, i.e. Arduino PINS 10(CS), 13(CLK), 11(DIN)
#define DISP8x8_CS  (1 << 2)
#define DISP8x8_CLK (1 << 5)
#define DISP8x8_DIN (1 << 3)

//change this if you are not Arduino Uno 
#define MISO (1 << 4)

//comment-out the following line for software version
#define USE_HW_VERSION

#ifdef USE_HW_VERSION
void send_spi_data(uint8_t reg, uint8_t data)
{
	DISP8x8_PORT &= ~DISP8x8_CS;  //SS down
	
	SPDR = reg;
	while (!((1 << SPIF) & SPSR));
	SPDR = data;
	while (!((1 << SPIF) & SPSR));
	
	DISP8x8_PORT |= DISP8x8_CS;  //SS up
}
#else
void send_spi_data(uint8_t reg, uint8_t data)
{
	int i;
	DISP8x8_PORT &= ~DISP8x8_CS;  //SS down

    uint16_t wrd = ((uint16_t)reg << 8) | (uint16_t)data;
	uint16_t mask = 32768;
	
	for (i = 0; i < 16; ++i)
	{
		if (wrd & mask) DISP8x8_PORT |= DISP8x8_DIN;
		else DISP8x8_PORT &= ~DISP8x8_DIN;
		mask >>= 1;	
				
		DISP8x8_PORT |= DISP8x8_CLK;
		_delay_us(1);
		DISP8x8_PORT &= ~DISP8x8_CLK;
		_delay_us(1);
	}
			
	DISP8x8_PORT |= DISP8x8_CS;  //SS up
}
#endif

void set_8x8_intensity(uint8_t intensity)
{
	send_spi_data(0x0a, intensity);
}

void init_8x8()
{
#ifdef USE_HW_VERSION
	DISP8x8_DDR &= ~MISO; // MISO as input
#endif
	DISP8x8_DDR |= DISP8x8_CS + DISP8x8_DIN + DISP8x8_CLK;  // SS, MOSI, SCK pin as output
	DISP8x8_PORT = DISP8x8_CS + MISO + DISP8x8_CLK; 
	wait(1);
	
	// SPI mode 0: leading edge = sample rising, trailing edge = setup falling
	// no interrupt, SPI enable, MSB first, master, mode 00, prescaler 16 (1 usec delay)
	SPSR = 0;

#ifdef USE_HW_VERSION	
	SPCR = 0b01010001;
#else
	SPCR = 0; 
#endif

	set_8x8_intensity(0x4);
	// scan limit 7
	send_spi_data(0x0b, 0x07);
	// decode mode 0 = led matrix (no digit decoding)
	send_spi_data(0x09, 0x0);
	// not shutdown mode
	send_spi_data(0x0c, 0x1);
	// not display test
	send_spi_data(0x0f, 0x0);
}

void set_8x8_segment(uint8_t segm, uint8_t data)
{
	send_spi_data(segm, data);
}

int main(void)
{
	init_terminal();
	printf("display8x8...");
	wait(1000);
    init_8x8();
    printf("initialized...");
	
	while(1)
    {
		for (int i = 1 ; i <= 8; i++)
		{
		  for (int j = 0; j < 8; j++) 
		  {
			  set_8x8_segment(i, 1 << j);
			  printf("%d ", i * 8 - 7 + j);
			  wait(100);
		  }
		  set_8x8_segment(i, 0);
		}
    }
}
