#ifndef __HART_PROTOCOL_PL_STDIO
#define __HART_PROTOCOL_PL_STDIO

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "HARTProtocolPL.hpp"


class HARTProtocolPLStdio : public HARTProtocolPL {
private:
	// Parameters of library
	static const bool INITIAL_STATE = true;			// 'true' -> complete transmition; 'false' -> partial transmition
	static const char HEXBYTE_DELIM = ' ';
	static const char COMMENT_CHAR = '#';
	static const char map_hex_to_char[16];			// ASCII hexadecimals converter helper

	FILE* fpin;
	FILE* fpout;

public:
	HARTProtocolPLStdio ();
	~HARTProtocolPLStdio();

	void RESETrequest ();
	bool RESETconfirm ();

	void ENABLErequest (bool state);
	bool ENABLEconfirm (bool& state);
	bool ENABLEindicate(bool& state);

	void DATArequest (char* frameData, unsigned int fDsize);
    	bool DATAconfirm ();
    	void DATAindicate(char* frameData, unsigned int& fDsize);

    	HART_Protocol_Errors_types& ERRORindicate (bool& status);
};

#endif