#include <stdio.h>
#include <wchar.h>
#include "serial_port.h"

// example: serial_open("com23", 9600)
HANDLE serial_open(LPWSTR portName, DWORD baudRate)
{
	wchar_t realPortNameBuf[20];
	LPWSTR realPortName = realPortNameBuf;

	if ((wcslen(portName) > 4) && (wcslen(portName) < 7) && (_wcsnicmp(portName, L"COM", 3) == 0)) 
		swprintf_s(realPortName, 20, L"\\\\.\\%s", portName); 
	else realPortName = portName;

	HANDLE handle = CreateFile(realPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("Error openning serial port, err=%d\n", GetLastError());
		return FALSE;
	}

	// Get current configuration of serial communication port.
	DCB config;
	if (GetCommState(handle, &config) == 0)
	{
		printf("Error getting port configuration, err=%d\n", GetLastError());
		return FALSE;
	}

	// assign user parameters
	config.BaudRate = baudRate;
	config.StopBits = 0;  // i.e. 1 stopbit
	config.Parity = 0;    // no parity
	config.ByteSize = 8;  // 8-bit byte

	if (SetCommState(handle, &config) == 0)
	{
		printf("Error setting port configuration, err=%d\n", GetLastError());
		return FALSE;
	}
	
	COMMTIMEOUTS comTimeOut;                
	// do not use read timeouts, readfile always blocks
	comTimeOut.ReadIntervalTimeout = 0;
	comTimeOut.ReadTotalTimeoutMultiplier = 0;
	comTimeOut.ReadTotalTimeoutConstant = 0;
	// do not use write timeouts
	comTimeOut.WriteTotalTimeoutMultiplier = 0;
	comTimeOut.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(handle, &comTimeOut);

	return handle;
}

int serial_close(HANDLE handle)
{
	if (0 == CloseHandle(handle))
	{
		printf("Error closing port, err=%d\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

DWORD serial_write(HANDLE handle, LPCVOID data, DWORD toWrite)
{
	DWORD written;
	if (0 == WriteFile(handle, data, toWrite, &written, 0))
	{
		printf("Error writing to port, err=%d\n", GetLastError());
		return FALSE;
	}
	return written;
}

DWORD serial_read(HANDLE handle, LPVOID data, DWORD toRead)
{
	DWORD bytesRead;
	BOOL rv = ReadFile(handle, data, toRead, &bytesRead, 0);
	if (rv == FALSE)
	{
		printf("Error reading from port, err=%d\n", GetLastError());
		return FALSE;
	}
	return bytesRead;
}
