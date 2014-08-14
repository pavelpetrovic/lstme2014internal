#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <Windows.h>

// opens a serial port
// example: serial_open("com23", 9600)
extern HANDLE serial_open(LPWSTR portName, DWORD baudRate);

// closes the serial port
extern int serial_close(HANDLE handle);

//returns the number of bytes that were written, blocks
extern DWORD serial_write(HANDLE handle, LPCVOID data, DWORD toWrite);

// returns the number of bytes that were read, blocks
extern DWORD serial_read(HANDLE handle, LPVOID data, DWORD toRead);

#endif  //_SERIAL_H_