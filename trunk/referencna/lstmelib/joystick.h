/*
 * joystick.h
 *
 */ 

#include <avr/io.h>

//setup ADC ports here (i.e. 0 for ADC0, 1 for ADC1, etc.)
#define JOY_X	0
#define JOY_Y	1

//PD6 is arduino D6
#define JOY_BUTTON_DDR	DDRD
#define JOY_BUTTON_PORT PORTD
#define JOY_BUTTON_PIN	PIND
#define JOY_BUTTON_IN	(1 << 6)

void joystick_init();

//returns 0-1023
uint16_t joystick_x();

//returns 0-1023
uint16_t joystick_y();

//returns 0-1
uint8_t joystick_button();
