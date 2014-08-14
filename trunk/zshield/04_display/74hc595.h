/*
 * 74hc595.h
 *
 * controls the 4 chained shift-registers that drive the 7-segment display
 * use disp_init() first then
 * disp_set_pin(0-3, 0-7, 0-1) to set/clear individual segment of the display in memory and then
 * disp_write() to make the changes visible 
 */ 


#ifndef _74HC595_H_
#define _74HC595_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define DISP_NUM_REGS 4

#define DISP_DDR1 DDRB
#define DISP_DDR2 DDRD
#define DISP_PORT1 PORTB
#define DISP_PORT2 PORTD

#define DISP_DATA  (1 << 3)
#define DISP_LATCH (1 << 2)
#define DISP_CLK (1 << 5)
#define DISP_OE (1 << 3)

void disp_init();
void disp_write();
void disp_set_pin(uint8_t reg, uint8_t pin, uint8_t val);
void disp_set_all(uint8_t val);
void disp_clear();


#endif /* _74HC595_H_ */