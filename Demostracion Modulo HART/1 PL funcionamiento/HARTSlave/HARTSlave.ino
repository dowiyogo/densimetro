#include "PackCommunication.h"
#include "HARTProtocolDeclarations.h"
#include "HARTProtocolErrors.h"
#include "HARTProtocolPL.h"
#include "HARTProtocolDLL.h"

#define SERIAL_BUFFER_SIZE  256
#define SERIAL3_BUFFER_SIZE 256

///////////////////////////////////////
// Declarations

char buff_bmodem[512];
int size_bmodem;
char buff_pc[512] = "default";
int size_pc       = 7;

///////////////////////////////////////
// Arduino Main

void binary_to_ASCII(char* destiny, const char* origin, int length)
{
	for (int i=0;i<length;i++) {
		unsigned char ls4b = origin[i] & 0x0F;
		unsigned char ms4b = ((unsigned char)(origin[i] & 0xF0)) >> 4;
		destiny[2*i]   = (ms4b < 0x0A)? (char) 48+ms4b:
						 				(char) 65+(ms4b-0x0A);
		destiny[2*i+1] = (ls4b < 0x0A)? (char) 48+ls4b:
						 				(char) 65+(ls4b-0x0A);
	}
}

void setup ()
{
	Serial.begin(9600);
 	Paco.begin(Serial);
	HARTProtocolPL::begin();
}

void loop ()
{
	char state = 0;

	for (;;) {
		// Physical Layer Test
		if (HARTProtocolPL::ENABLEindicate()) {
			HARTProtocolPL::DATAindicate(buff_bmodem, size_bmodem);

			HARTProtocolPL::ENABLErequest();
			HARTProtocolPL::DATArequest(buff_pc, size_pc);
			HARTProtocolPL::DATAconfirm();

			Paco.send(buff_bmodem, size_bmodem);
		}
		if (Paco.recv(buff_bmodem, size_bmodem)) {
			for (int i=0; i<size_bmodem; i++) {
				buff_pc[i] = buff_bmodem[i];
				size_pc    = size_bmodem;
			}
		}
	}
}
