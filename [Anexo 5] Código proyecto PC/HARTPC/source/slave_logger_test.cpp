#include <stdio.h>
#include "slave_logger.hpp"

int main(int argc, char* argv[]){
	HARTSlaveLogger serial("/dev/ttyUSB0");
	
	return 0;
}