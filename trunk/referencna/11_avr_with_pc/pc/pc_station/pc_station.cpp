#include <stdio.h>

#include "serial_port.h"


int main()
{
	char msg[100];
	int n;
	
	printf("simple line terminal, enter quit to exit.\n");
	printf("connecting...");
	HANDLE comX = serial_open(L"COM12", 76800U);
	if (comX == FALSE) return 1;
	Sleep(2500);
	serial_write(comX, "..!", 3);
	do { serial_read(comX, msg, 1); } while (msg[0] != '@');
	printf("link alive.\n");
		
	do {
		printf("msg>");
		fgets(msg, 99, stdin);
		n = strlen(msg);
		msg[n - 1] = 0;
		n--;

		int written = serial_write(comX, msg, n);
		serial_write(comX, "^", 1);
		int nread = serial_read(comX, msg, n);
		
		printf("rep>%s\n", msg);
		if (strncmp(msg, "tiuq", 4) == 0) break;
		serial_read(comX, msg, 1); // ^
	} while (1);

	serial_close(comX);

	printf("Press enter to terminate...");
	char buf[4];
	fgets(buf, 3, stdin);

	return 0;
}