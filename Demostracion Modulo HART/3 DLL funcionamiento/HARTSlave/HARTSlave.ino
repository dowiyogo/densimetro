#include <PackCommunication.h>
#include "HARTProtocolDeclarations.h"
#include "HARTProtocolErrors.h"
#include "HARTProtocolPL.h"
#include "HARTProtocolDLL.h"

///////////////////////////////////////
// Declarations
HARTProtocolDLL hart;
HARTCommand command;
HART_Protocol_Errors_types error;
char buff[256];
unsigned char size_b;
int _size_b;
char buff_pc[512]      = "default";
unsigned char size_bpc = 7;

///////////////////////////////////////
// Arduino Main

void setup ()
{
 	Paco.begin(Serial, 9600);
	HARTProtocolPL::begin();
}

void loop ()
{
	char state = 0;

	for (;;) {
		// Link Layer Test
		if (hart.TRANSMITindicate(command, buff, size_b)) {
			error = HARTProtocolErrors.get();
			if (error == NO_ERROR || error == RCV_BAD_CHECK_BYTE || error == COMMAND_NOT_RECEIVED || error == DATA_NOT_RECEIVED || error ==	CHECK_BYTE_NOT_RECEIVED) {
				hart.TRANSMITresponse(command, buff_pc, size_bpc);
			}

			char e = (char)error;
			Paco.send((char*)&e, 1);
		}
		if (Paco.recv(buff, _size_b)) {
			for (int i=0; i<_size_b; i++) {
				buff_pc[i] = buff[i];
			}
			size_bpc = (unsigned char) _size_b;
		}
	}
}
