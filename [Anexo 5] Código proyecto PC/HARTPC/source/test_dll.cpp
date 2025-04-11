#include <vector>

#include <stdio.h>

#include "HARTProtocolDeclarations.hpp"
#include "HARTProtocolDLL.hpp"
#include "HARTProtocolErrors.hpp"

#include "tester.h"

HARTProtocolDLL hart(PL_CHANNEL_STDIO);
HARTCommand command;
char data[256] = {};
unsigned char data_size = 1;

extern HART_Protocol_Errors HARTProtocolErrors;

/* Unit Tests */
int HPDLLTransmit_ind();
int HPDLLTransmit_bad_preamble_size();
int HPDLLTransmit_no_start_byte();
int HPDLLTransmit_delim_wrong_modulation();
int HPDLLTransmit_delim_expans_bytes_not_zero();
int HPDLLTransmit_addr_bad_polling();
int HPDLLTransmit_addr_bad_unique();
int HPDLLTransmit_delim_polling_and_not_c0();
int HPDLLTransmit_data_not_enough();
int HPDLLTransmit_checksum_bad();


int main(int argc, char* argv[])
{
	TESTER_NAME("HART Protocol Test");

	TESTER_TEST("DLL :: Transmit Indicate", HPDLLTransmit_ind);
	TESTER_TEST("DLL :: Transmit (preamble) bad preamble size", HPDLLTransmit_bad_preamble_size);
	TESTER_TEST("DLL :: Transmit (start byte) no start byte", HPDLLTransmit_no_start_byte);
	TESTER_TEST("DLL :: Transmit (delim) wrong modulation -> Not FSK", HPDLLTransmit_delim_wrong_modulation);
	TESTER_TEST("DLL :: Transmit (delim) not 0 expansion bytes", HPDLLTransmit_delim_expans_bytes_not_zero);
	TESTER_TEST("DLL :: Transmit (adrress) bad polling address", HPDLLTransmit_addr_bad_polling);
	TESTER_TEST("DLL :: Transmit (address) bad unique address", HPDLLTransmit_addr_bad_unique);
	TESTER_TEST("DLL :: Transmit (delim&command) polling address command 1", HPDLLTransmit_delim_polling_and_not_c0);
	TESTER_TEST("DLL :: Transmit (data) not enough data", HPDLLTransmit_data_not_enough);
	TESTER_TEST("DLL :: Transmit (checksum) bad checksum calculation", HPDLLTransmit_checksum_bad);
	TESTER_TEST("DLL :: Transmit (checksum) too much data", HPDLLTransmit_checksum_bad);
	
	return 0;
}

int HPDLLTransmit_ind()
{
	hart.TRANSMITindicate(command, data, data_size);
	if (HARTProtocolErrors.get() != NO_ERROR)
		return 1;
	else if (command != COMMAND0)
		return 2;
	else if (data_size != 0)
		return 3;

	return 0;
}
int HPDLLTransmit_bad_preamble_size()
{
	hart.TRANSMITindicate(command, data, data_size);
	if (HARTProtocolErrors.get() == PREAMBLE_BYTES_NOT_RECEIVED)
		return 1;
	else if (HARTProtocolErrors.get() < PREAMBLE_VALUE_ERROR)
		return 2;

	return 0;
}
int HPDLLTransmit_no_start_byte()
{
	hart.TRANSMITindicate(command, data, data_size);
	if (HARTProtocolErrors.get() == START_MESSAGE_NOT_RECEIVED)
		return 1;
	else if (HARTProtocolErrors.get() != START_MESSAGE_VALUE_ERROR)
		return 2;

	return 0;
}
int HPDLLTransmit_delim_wrong_modulation()
{
	hart.TRANSMITindicate(command, data, data_size);
	if(HARTProtocolErrors.get() == DELIMETER_BYTES_NOT_RECEIVED)
		return 1;
	else if(HARTProtocolErrors.get() != FRAME_NOT_FSK)
		return 2;

	return 0;
}
int HPDLLTransmit_delim_expans_bytes_not_zero()
{
	hart.TRANSMITindicate(command, data, data_size);
	if(HARTProtocolErrors.get() == DELIMETER_BYTES_NOT_RECEIVED)
		return 1;
	else if(HARTProtocolErrors.get() != FRAME_NOT_ZERO_EXPANS)
		return 2;

	return 0;
}
int HPDLLTransmit_addr_bad_polling()
{
	hart.TRANSMITindicate(command, data, data_size);
	if(HARTProtocolErrors.get() == PADDRESS_NOT_RECEIVED)
		return 1;
	else if(HARTProtocolErrors.get() != RCV_ADDRESS_NOT_MATCH)
		return 2;

	return 0;
}
int HPDLLTransmit_addr_bad_unique()
{
	hart.TRANSMITindicate(command, data, data_size);
	if(HARTProtocolErrors.get() == UADDRESS_NOT_RECEIVED)
		return 1;
	else if(HARTProtocolErrors.get() != RCV_ADDRESS_NOT_MATCH)
		return 2;

	return 0;
}
int HPDLLTransmit_delim_polling_and_not_c0()
{
	hart.TRANSMITindicate(command, data, data_size);
	if(HARTProtocolErrors.get() == COMMAND_NOT_RECEIVED)
		return 1;
	else if(HARTProtocolErrors.get() != RCV_ADDRESS_NOT_MATCH)
		return 2;

	return 0;
}
int HPDLLTransmit_data_not_enough()
{
	hart.TRANSMITindicate(command, data, data_size);
	if (HARTProtocolErrors.get() != BYTE_COUNT_NOT_RECEIVED && HARTProtocolErrors.get() != DATA_NOT_RECEIVED)
		return 1;

	return 0;
}
int HPDLLTransmit_checksum_bad()
{
	hart.TRANSMITindicate(command, data, data_size);
	if (HARTProtocolErrors.get() == CHECK_BYTE_NOT_RECEIVED)
		return 1;
	else if(HARTProtocolErrors.get() != RCV_BAD_CHECK_BYTE)
		return 2;

	return 0;
}