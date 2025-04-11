#ifndef __HART_PROTOCOL_PL
#define __HART_PROTOCOL_PL

#include <Boards.h>
#include "PackCommunication.h"
#include "HARTProtocolDeclarations.h"


namespace HARTProtocolPL {

///////////////////////////////////////
// PARAMETERS

const int pin_RTS = 7;	// USING !RTS (~RTS) as required input in modem
const int pin_CD  = 6;
const int pin_CTS = 5;
const int pin_RESET = 4;

const int tcoff  = 4;
const int tcdoff = 6;
const int tcon   = 4;
const int tcdon  = 6;

///////////////////////////////////////
// Declarations

void RESETrequest();
//bool RESETconfirm();
void begin();
void ENABLErequest();
bool ENABLEindicate();
bool ENABLEconfirm();
void DATArequest(char * data, int size);
void DATAconfirm();
void DATAindicate(char * data, int & size);
//inline bool ERRORindicate(bool status, char * data, unsigned char size);

}

#endif // __HART_PROTOCOL_PL
