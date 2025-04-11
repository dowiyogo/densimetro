#ifndef __SLAVE_LOGGER_H
#define __SLAVE_LOGGER_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#define DEFAULT_BSIZE 256

class HARTSlaveLogger{
private:
	FILE* fd;
	char buffer[DEFAULT_BSIZE];

public:
	HARTSlaveLogger();
	~HARTSlaveLogger();

	int read();
	int read(char* destiny, int size);
};

#endif // __SLAVE_LOGGER_H