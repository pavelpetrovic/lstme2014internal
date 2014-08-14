#ifndef __FOUR_DIGIT_DISPLAY_H__
#define __FOUR_DIGIT_DISPLAY_H__

#include <avr/io.h>

// where do you connect your 4-digit display?
#define DISP_PORT	PORTD
#define DISP_PIN	PIND
#define DISP_DDR	DDRD
#define DISP_CLK	(1 << 2) 				
#define DISP_DIO	(1 << 3)				

#define ADDR_FIXED 0x44
#define STARTADDR  0xc0 
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   1
#define POINT_OFF  0
/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

void four_digit_init();                // init & clear

void four_digit_set_brightness(uint8_t b);						// set before calling display
void four_digit_set_point(uint8_t disp_point);                 // set before calling display
void four_digit_display(uint8_t pos, uint8_t digit);		// display digit 0-f at position 0-3 
void four_digit_clear();									// clear whole display

#endif
