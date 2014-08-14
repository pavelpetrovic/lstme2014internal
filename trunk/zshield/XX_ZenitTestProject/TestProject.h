#ifndef ZENIT2014_H_
#define ZENIT2014_H_

// Shield pin configuration

#define DISP_DATA_PORT	B
#define DISP_DATA_BIT	3
#define DISP_CLK_PORT	B
#define DISP_CLK_BIT	5
#define DISP_LATCH_PORT	B
#define DISP_LATCH_BIT	2
#define DISP_OE_PORT	D
#define DISP_OE_BIT		3

#define BUZZER_PORT		B
#define BUZZER_BIT		1

#define TEMP_ALERT_PORT	D
#define TEMP_ALERT_BIT	2

#define LED1_PORT		B
#define LED1_BIT		0
#define LED2_PORT		D
#define LED2_BIT		7
#define LED3_PORT		D
#define LED3_BIT		6
#define LED4_PORT		D
#define LED4_BIT		5

#define BUTTON1_PORT	C
#define BUTTON1_BIT		0
#define BUTTON2_PORT	D
#define BUTTON2_BIT		0
#define BUTTON3_PORT	D
#define BUTTON3_BIT		1
#define BUTTON4_PORT	D
#define BUTTON4_BIT		4

#define POT_CHANNEL		2
#define PHOTO_CHANNEL	3

#define DAC_ADDRESS		0b11000000
#define TEMPSENS_ADDRESS	0b10010000

// ========================================================================= //

// General macros
#define sbit(var, bit)	var |= (1 << bit)
#define cbit(var, bit)	var &= ~(1 << bit)
#define rbit(var, bit)	((var & (1 << bit)) != 0)
#define CONCAT(a, b)	a ## b
#define DDR(port)		CONCAT(DDR, port)
#define INPORT(port)	CONCAT(PIN, port)
#define OUTPORT(port)	CONCAT(PORT, port)

#endif /* ZENIT2014_H_ */