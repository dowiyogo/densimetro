#include "slave_logger.hpp"

HARTSlaveLogger::HARTSlaveLogger(char* path)
{
	fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	fcntl(fd, F_SETFL,0);
}
HARTSlaveLogger::~HARTSlaveLogger()
{
	close(fd);
}

int HARTSlaveLogger::read(char* destiny, int size)
{
	return read(fd, buff, size);
}