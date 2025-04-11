#include <PackCommunication.h>
#include "HARTProtocolDeclarations.h"
#include "HARTProtocolErrors.h"
#include "HARTProtocolPL.h"
#include "HARTProtocolDLL.h"

///////////////////////////////////////
// Declarations
//extern PackCommunication Paco;
HARTProtocolDLL hart;
HARTCommand command;
char buff[256];
unsigned char size_b;

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
			hart.TRANSMITresponse(command, buff, size_b);
			Paco.send("t", 1);
			Paco.send(buff, size_b);
		}
		if (HARTProtocolErrors.get() != NO_ERROR) {
			Paco.send("e", 1);
			Paco.send("Error", 5); // Expand to detect error !
		}
	}
}
