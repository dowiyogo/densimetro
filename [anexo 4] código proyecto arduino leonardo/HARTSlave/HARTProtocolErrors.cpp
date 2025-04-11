#include "HARTProtocolErrors.h"

/******** GLOBAL OBJECT INICIALIZATION **********/

HART_Protocol_Errors HARTProtocolErrors;



HART_Protocol_Errors::HART_Protocol_Errors ()
{
	clear();
}
HART_Protocol_Errors::~HART_Protocol_Errors () {}

void HART_Protocol_Errors::set(HART_Protocol_Errors_types error)
{
	stack = error;
}

HART_Protocol_Errors_types HART_Protocol_Errors::get ()
{
	return stack;
}

void HART_Protocol_Errors::clear ()
{
	stack = NO_ERROR;
}

/*std::string HART_Protocol_Errors::toString ()
{
	return errors_in_strings[stack];
}*/
