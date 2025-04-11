#include "HARTProtocolErrors.hpp"

/******** GLOBAL OBJECT INICIALIZATION **********/

HART_Protocol_Errors HARTProtocolErrors;

/******** Private constant arrays ***************/

const std::string HART_Protocol_Errors::errors_in_strings[] = {
	"NO_ERROR",
	/***** DLL - LLC *****/
	// Not received
	"NO_BYTES_FB",
	"PREAMBLE_BYTES_NOT_RECEIVED",
	"START_MESSAGE_NOT_RECEIVED",
	"DELIMETER_BYTES_NOT_RECEIVED",
	"PADDRESS_NOT_RECEIVED",
	"UADDRESS_NOT_RECEIVED",
	"EXPANSION_NOT_RECEIVED",
	"COMMAND_NOT_RECEIVED",
	"BYTE_COUNT_NOT_RECEIVED",
	"DATA_NOT_RECEIVED",
	"CHECK_BYTE_NOT_RECEIVED",
	// No Buffer
	"BUFFER_NOT_SENDED",
	"BUFFER_NOT_RCVED",
	// Value not matched
	"PREAMBLE_VALUE_ERROR",
	"START_MESSAGE_VALUE_ERROR",
	"FRAME_NOT_FSK",
	"FRAME_NOT_ADDR_TYPE",
	"FRAME_NOT_ZERO_EXPANS",
	"FRAME_NOT_FROM_XMASTER",
	"RCV_ADDRESS_NOT_MATCH",
	"RCV_BAD_CHECK_BYTE",
	// Stack in use
	"RCV_STACK_IN_USE",
	"SEND_STACK_IN_USE",

	"NOT_DEFINED_ERROR"
};


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

std::string HART_Protocol_Errors::toString ()
{
	return errors_in_strings[stack];
}
