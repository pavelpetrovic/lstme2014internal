/*
 * wait.h
 *
 *  utility function to wait given number of milliseconds
 */ 

#ifndef WAIT_H_
#define WAIT_H_

#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>

void wait(uint16_t ms);

#endif /* WAIT_H_ */