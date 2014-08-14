/*
 * shared/terminal.h
 *
 * configures serial port 0 (USART0) as standard input/output
 * with config parameters 76800-8-N-1
 */ 


#ifndef TERMINAL_H_
#define TERMINAL_H_

extern uint8_t terminal_echo_on;

void init_terminal();
uint8_t char_ready();

#endif /* TERMINAL_H_ */