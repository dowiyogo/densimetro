#include <PackCommunication.h>
#include "HARTProtocolDeclarations.h"
#include "HARTProtocolErrors.h"
#include "HARTProtocolPL.h"
#include "HARTProtocolDLL.h"
#include "HARTProtocolAL.h"

///////////////////////////////////////
// Declarations
HARTProtocolAL hart_al;
HARTProtocolDLL hart_dll;
HARTCommand command;
HART_Protocol_Errors_types error;
char buff[256];
unsigned char size_b;
int _size_b;
char buff_pc[512] = "default";
int size_bpc      = 7;

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
		if (hart_dll.TRANSMITindicate(command, buff, size_b)) {
			error = HARTProtocolErrors.get();
			if (error == NO_ERROR) {
				hart_al.setData(command, buff, size_b);
				hart_dll.TRANSMITresponse(command, buff, size_b);
				size_b = 0;
			}
			else if ( error == RCV_BAD_CHECK_BYTE || error == COMMAND_NOT_RECEIVED) {
				hart_dll.TRANSMITresponse(command, buff, size_b);
			}

			char e = (char)error;
			Paco.send((char*)&e, 1);
		}
		_size_b = (int)size_b;
		if (Paco.recv(buff, _size_b)) {
			for (int i=0; i<size_b; i++) {
				buff_pc[i] = buff[i];
				size_bpc   = size_b;
			}
		}
	}
}
