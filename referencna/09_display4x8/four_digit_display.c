
#include <util/delay.h>
#include "four_digit_display.h"

uint8_t Cmd_DispCtrl  = 0;
uint8_t _PointFlag    = 0;     //_PointFlag=1:the clock point on

const int8_t TubeTab[] =
{
    0x3f,0x06,0x5b,0x4f,
    0x66,0x6d,0x7d,0x07,
    0x7f,0x6f,0x77,0x7c,
    0x39,0x5e,0x79,0x71
}; //0~9,A,b,C,d,E,F

// write a byte
void writeByte(int8_t wr_data)
{
    uint8_t i; 
    for(i = 0; i < 8; i++)                                  //sent 8bit data
    {
        DISP_PORT &= ~DISP_CLK; 
		if (wr_data & 0x01) DISP_PORT |= DISP_DIO;  
        else DISP_PORT &= ~DISP_DIO;  
        wr_data >>= 1;
        _delay_us(10);
        DISP_PORT |= DISP_CLK;  
		_delay_us(10);
    }
    DISP_PORT &= ~DISP_CLK;  
	DISP_PORT |= DISP_DIO;  
    _delay_us(1);
	DISP_PORT |= DISP_CLK;  
	_delay_us(1);
	DISP_DDR &= ~DISP_DIO;  
    
	while (DISP_PIN & DISP_DIO) 
    {
		_delay_us(10);
        DISP_DDR |= DISP_DIO; 
        DISP_PORT &= ~DISP_DIO;  
        DISP_DDR &= ~DISP_DIO;   
    }
	_delay_us(1);
    DISP_DDR |= DISP_DIO;  
}

// send start signal to TM1637
void start(void)
{
	DISP_PORT |= DISP_CLK; 
	_delay_us(1);
	DISP_PORT |= DISP_DIO; 
	_delay_us(2);
	DISP_PORT &= ~DISP_DIO; 
	_delay_us(2);
	DISP_PORT &= ~DISP_CLK; 
}

// End of transmission
void stop(void)
{
	DISP_PORT &= ~DISP_CLK; 
	_delay_us(1);
	DISP_PORT &= ~DISP_DIO; 
	_delay_us(2);
	DISP_PORT |= DISP_CLK; 
	_delay_us(2);
	DISP_PORT |= DISP_DIO; 
}

uint8_t coding(int8_t DispData)
{
    uint8_t PointData;
    if(_PointFlag == POINT_ON)PointData = 0x80;
    else PointData = 0;
    if(DispData == 0x7f) DispData = 0x00 + PointData; 
    else DispData = TubeTab[DispData] + PointData;
    return DispData;
}

// initialize
void four_digit_init()
{
	DISP_DDR &= ~(DISP_CLK + DISP_DIO);
	DISP_PORT = 0;        //pull-ups off
	_delay_us(10);
	DISP_DDR |= DISP_CLK + DISP_DIO;   //clk:PD2, data:PD3
	DISP_PORT |= DISP_CLK + DISP_DIO;  // set high on idle
    four_digit_clear();
}

// set brightness
void four_digit_set_brightness(uint8_t b)           //set before calling display
{
    Cmd_DispCtrl = 0x88 + b;
}

// show :
void four_digit_set_point(uint8_t disp_point)              //set before calling display
{
    _PointFlag = disp_point;
}

// display a bit
void four_digit_display(uint8_t pos, uint8_t digit)
{
    uint8_t SegData;
    SegData = coding(digit);
    start();          
    writeByte(0x44);
    stop();           
    start();          
    writeByte(pos | 0xc0);
    writeByte(SegData);
    stop();            
    start();          
    writeByte(Cmd_DispCtrl);
    stop();           
}

// clear display
void four_digit_clear()
{
    four_digit_display(0x00, 0x7f);
    four_digit_display(0x01, 0x7f);
    four_digit_display(0x02, 0x7f);
    four_digit_display(0x03, 0x7f);
}
