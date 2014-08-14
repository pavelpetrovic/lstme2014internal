/* Zenit 2014 Test project
 * Author: xpavlicek@gmail.com
*/
#define F_CPU	16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "TestProject.h"
#include "i2c_master.h"

#define button1Pressed()	!rbit(INPORT(BUTTON1_PORT), BUTTON1_BIT)
#define button2Pressed()	!rbit(INPORT(BUTTON2_PORT), BUTTON2_BIT)
#define button3Pressed()	!rbit(INPORT(BUTTON3_PORT), BUTTON3_BIT)
#define button4Pressed()	!rbit(INPORT(BUTTON4_PORT), BUTTON4_BIT)

// Segment order: DP B C A G D F E
volatile uint8_t charmap[] = {
	0b10001000,	// 0
	0b10011111,	// 1
	0b10100010,	// 2
	0b10000011,	// 3
	0b10010101,	// 4
	0b11000001,	// 5
	0b11000000,	// 6
	0b10001111,	// 7
	0b10000000,	// 8
	0b10000001,	// 9
	0b10000100,	// A
	0b11010000,	// b
	0b11101000,	// C
	0b10010010,	// d
	0b11100000,	// E
	0b11100100,	// F
	0b11111111,	// blank
	0b11110111,	// -
	0b10100101,	// °
	0b10100100	// P
};

#define DIGIT_BLANK	16
#define DIGIT_MINUS	17
#define DIGIT_DEGREE	18
#define DIGIT_P	19

/* ========================================================================== */

// Write 32 bits to display register and latch
void writeDisplayData(uint32_t data)
{
	// shift out data
	for (uint8_t i = 0; i < 4; i++)
	{
		// send byte over SPI
		SPDR = data >> (8 * (3 - i));
		// wait for transmission complete
		while(!(SPSR & (1 << SPIF)));
	}

	// latch register data
	sbit(OUTPORT(DISP_LATCH_PORT), DISP_LATCH_BIT);
	_delay_us(10);
	cbit(OUTPORT(DISP_LATCH_PORT), DISP_LATCH_BIT);
}

/* ========================================================================== */

// Convert digits to data for display using charmap array
uint32_t convertBcd(uint8_t digit3, uint8_t digit2, uint8_t digit1, uint8_t digit0, uint8_t decimalPoint)
{
	uint32_t data = 0;

	data |= charmap[digit3];
	if (decimalPoint == 3) data &= 0xFFFFFF7F;
	data <<= 8;

	data |= charmap[digit2];
	if (decimalPoint == 2) data &= 0xFFFFFF7F;
	data <<= 8;

	data |= charmap[digit1];
	if (decimalPoint == 1) data &= 0xFFFFFF7F;
	data <<= 8;

	data |= charmap[digit0];
	if (decimalPoint == 0) data &= 0xFFFFFF7F;

	return data;
}

/* ========================================================================== */

// Write number to display
void writeDisplayNumber(uint16_t number, uint8_t decimalPoint)
{
	uint8_t digit0;
	uint8_t digit1;
	uint8_t digit2;
	uint8_t digit3;

	// convert digits
	digit3 = number / 1000;
	number %= 1000;
	digit2 = number / 100;
	number %= 100;
	digit1 = number / 10;
	number %= 10;
	digit0 = number;

	// strip leading zeros
	if (digit3 == 0 && decimalPoint < 3)
	{
		digit3 = DIGIT_BLANK;
		if (digit2 == 0 && decimalPoint < 2)
		{
			digit2 = DIGIT_BLANK;
			if (digit1 == 0 && decimalPoint < 1)
			{
				digit1 = DIGIT_BLANK;
			}
		}
	}

	// convert and write to display
	uint32_t data = convertBcd(digit3, digit2, digit1, digit0, decimalPoint);
	writeDisplayData(data);
}

// Write number to display
void writeDisplayTemp(uint16_t number, uint8_t decimalPoint)
{
	uint8_t digit0;
	uint8_t digit1;
	uint8_t digit2;
	uint8_t digit3;

	// convert digits
	digit3 = number / 1000;
	number %= 1000;
	digit2 = number / 100;
	number %= 100;
	digit1 = number / 10;
	number %= 10;
	digit0 = number;

	// strip leading zeros
	if (digit3 == 0 && decimalPoint < 3)
	{
		digit3 = DIGIT_BLANK;
		if (digit2 == 0 && decimalPoint < 2)
		{
			digit2 = DIGIT_BLANK;
			if (digit1 == 0 && decimalPoint < 1)
			{
				digit1 = DIGIT_BLANK;
			}
		}
	}

	// convert and write to display
	uint32_t data = convertBcd(digit2, digit1, digit0, DIGIT_DEGREE, decimalPoint);
	writeDisplayData(data);
}

/* ========================================================================== */

// Run single A/D conversion and return result
uint16_t readAnalog(uint8_t channel)
{
	// Select channel
	ADMUX = (ADMUX & 0xF0) | channel;
	// Start the AD conversion
	ADCSRA |= (1 << ADSC);
	// Wait for the AD conversion to complete
	while ((ADCSRA & (1 << ADIF)) == 0);
	// Clear flag
	ADCSRA |= (1 << ADIF);
	return ADC;
}

/* ========================================================================== */

// Read temperature from temperature sensor
// @return	Signed integer of 10x temperature value, e.g. +245 for 24.5°C
int16_t readTemperature()
{		
	// write temperature register pointer
	i2c_start(TEMPSENS_ADDRESS, I2C_WRITE);
	i2c_write(0x00);

	// read temperature register
	uint16_t t = 0;
	i2c_start(TEMPSENS_ADDRESS, I2C_READ);
	t = i2c_read_Ack();
	t <<= 8;
	t |= i2c_read_nAck();
	i2c_stop();

	// get and clear sign
	int16_t val = (t & 0x8000) ? -1 : 1;
	t &= 0x7FFF;

	// shift and round value
	t >>= 4;
	t *= 10;
	t += 8;
	t >>= 4;

	// apply sign
	val *= t;
	return val;
}

/* ========================================================================== */

// Setup MCU input/output
void init()
{
	// Display connection pins output
	sbit(DDR(DISP_DATA_PORT), DISP_DATA_BIT);
	sbit(DDR(DISP_CLK_PORT), DISP_CLK_BIT);
	sbit(DDR(DISP_LATCH_PORT), DISP_LATCH_BIT);
	sbit(DDR(DISP_OE_PORT), DISP_OE_BIT);

	// SPI configuration: master, sck=fclk/128=125kHz
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0);

	// LEDs output
	sbit(DDR(LED1_PORT), LED1_BIT);
	sbit(DDR(LED2_PORT), LED2_BIT);
	sbit(DDR(LED3_PORT), LED3_BIT);
	sbit(DDR(LED4_PORT), LED4_BIT);
	
	// Button pull-ups
	sbit(OUTPORT(BUTTON1_PORT), BUTTON1_BIT);
	sbit(OUTPORT(BUTTON2_PORT), BUTTON2_BIT);
	sbit(OUTPORT(BUTTON3_PORT), BUTTON3_BIT);
	sbit(OUTPORT(BUTTON4_PORT), BUTTON4_BIT);
	
	// Buzzer timer output
	sbit(DDR(BUZZER_PORT), BUZZER_BIT);
	TCCR1A = (1<<COM1A1) | (1<<COM1A0) | (1<<WGM11);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11);
	ICR1 = 0;		// timer top value
	OCR1A = 500;	// output compare value

	// ADC initialization
	// - ADC clock frequency: F_CPU / 128 (125kHz @ 16MHz)
	// - reference: AVcc with external cap
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	// init I2C interface
	i2c_init();
}

/* ========================================================================== */

void mode3()
{
	uint8_t t = 100;
	uint8_t esc[3] = {255, 255, 255};

	// wait for start
	writeDisplayNumber(t, 1);

	while (!button4Pressed());
	
	uint8_t swlast = 0;
	
	while (1)
	{
		t--;
		writeDisplayNumber(t, 1);
		
		if (button4Pressed() && swlast == 0)
		{
			swlast = 1;
			
			esc[2] = esc[1];
			esc[1] = esc[0];
			esc[0] = t;
			
			// deactivate
			if (esc[2] - esc[0] < 10) break;
		}
		else if (!button4Pressed() && swlast == 1)
		{
			swlast = 0;
		}
		
		
		uint8_t f = t % 10;
		
		OCR1A = 500;
		
		switch (f)
		{
			case 0:
				// turn-on buzzer
				ICR1 = 1000;				
				break;
			case 9:
				// turn-off buzzer
				ICR1 = 0;				
				break;

			case 5:
				if (t < 50 && t > 20)
				{
					ICR1 = 1000;
				}
				if (t < 20)
				{
					ICR1 = 0;
				}
				break;
			case 4:
				if (t < 50 && t > 20)
				{
					ICR1 = 0;
				}
				if (t < 20)
				{
					ICR1 = 1000;
				}
				break;
			case 8:
			case 6:
			case 2:
			if (t < 20)
			{
				ICR1 = 1000;
			}
			break;
			case 7:
			case 3:
			case 1:
			if (t < 20)
			{
				ICR1 = 0;
			}
			break;

		}
		
		if (t == 0) break;
		
		_delay_ms(100);
	}

	
	if (t == 0)
	{
		sbit(OUTPORT(LED4_PORT), LED4_BIT);
	}
	else
	{
		ICR1 = 0;
		
	}

	while (!button3Pressed());
	
}


/* ========================================================================== */

#define MODE_1	0
#define MODE_2	1
#define MODE_3	2

volatile uint8_t mode = MODE_1;

uint8_t mydelay = 0;

int main(void)
{
	// init hardware setup
	init();

	// setup temperature resolution
	// write temperature register pointer
	i2c_start(TEMPSENS_ADDRESS, I2C_WRITE);
	i2c_write(0x01);
	i2c_write(0b01100000);
	i2c_stop();

    while(1)
    {
		// change mode by pressed button
		if (button1Pressed())
		{
			mode = MODE_1;
		}
		else if (button2Pressed())
		{
			mode = MODE_2;
		}
		else if (button3Pressed())
		{
			mode = MODE_3;
		}

		// mode1
		if (mode == MODE_1)
		{
			// read analog value from potentiometer
			uint32_t pot = readAnalog(POT_CHANNEL);
			pot *= 4888;
			pot += 500;
			pot /= 1000;

			if (++mydelay >= 10)
			{
				writeDisplayNumber(pot, 0);
				mydelay = 0;
			}
			

			// led1 on
			sbit(OUTPORT(LED1_PORT), LED1_BIT);
		}
		else
		{
			// led1 off
			cbit(OUTPORT(LED1_PORT), LED1_BIT);			
		}

		// mode2
		if (mode == MODE_2)
		{

			int16_t temp = readTemperature();
			writeDisplayTemp(temp, 2);

			// led2 on
			sbit(OUTPORT(LED2_PORT), LED2_BIT);
		}
		else
		{
			// led2 off
			cbit(OUTPORT(LED2_PORT), LED2_BIT);
		}

		// show potentiometer value		
		if (mode == MODE_3)
		{
			// led3 on
			sbit(OUTPORT(LED3_PORT), LED3_BIT);
			
			mode3();
			
			/*
			if (mydelay < 12)
			{
				// turn-on buzzer
				// frequency 2kHz
				ICR1 = 2000;
				OCR1A = 1000;
				// write to display
				writeDisplayData(convertBcd(DIGIT_MINUS, DIGIT_MINUS, DIGIT_BLANK, DIGIT_BLANK, 255));
			}
			else
			{
				// turn-on buzzer
				// frequency 2kHz
				ICR1 = 4000;
				OCR1A = 2000;
				// write to display
				writeDisplayData(convertBcd(DIGIT_BLANK, DIGIT_BLANK, DIGIT_MINUS, DIGIT_MINUS, 255));			}
			if (++mydelay >= 25) mydelay = 0;
			*/
		}
		else
		{
			// turn-off buzzer
			ICR1 = 0;
			// led3 off
			cbit(OUTPORT(LED3_PORT), LED3_BIT);
		}


		// wait until all mode buttons are released
		while (button1Pressed() || button2Pressed() || button3Pressed());
		_delay_ms(20);
    }
}