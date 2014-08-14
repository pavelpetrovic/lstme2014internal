/*
 * PotPhoto.c
 *
 * demonstrates using analogue sensors on z-shield: potenciometer and phototransistor
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "terminal.h"
#include "wait.h"

void init_pot_photo()
{
	//ref. voltage: AVCC with external capacitor at AREF pin, select ADC0 x channel
	//adc on, no interrupt, no trigger, prescaler 128
	ADCSRA = 0b10000111;
	ADCSRB = 0;
	//disable digital input pin register on analog inputs a2, a3
	DIDR0 = 0b00001100;		
}


int main(void)
{
	uint16_t cnt = 0;
	init_terminal();
	init_pot_photo();
	printf("Showing values of analogue sensors on shield (potenciometer and phototransistor)\n\r");
	
    while(1)
    {
        ADMUX = 0b01000010; //select pot channel
        ADCSRA |= (1 << ADSC); // start conversion
        while (0 == (ADCSRA & (1 << ADIF))); // wait for the conversion to be ready
        uint16_t x = ADC;
        
        ADMUX = 0b01000011; //select photo channel
        ADCSRA |= (1 << ADSC); // start conversion
        while (0 == (ADCSRA & (1 << ADIF))); // wait for the conversion to be ready
        uint16_t y = ADC;
        
        printf("%u %u %u\r\n", cnt, x, y);
		cnt++;
        wait(300);
    }
	return 0;
}
