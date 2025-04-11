#ifndef __HART_PROTOCOL_AL
#define __HART_PROTOCOL_AL

#include "HARTProtocolDeclarations.h"
#include "HARTProtocolDLL.h"

class HARTProtocolAL{
private://Class Parameters

public://Interface

	HARTProtocolAL();
	~HARTProtocolAL();

	void setData(HARTCommand command, char* data, unsigned char& size);

private:

	HARTProtocolDLL dll;

	// HART status
	char message[24];
	char tag[6];
	char descriptor[8];
	char date[3];

	// Measurement variable
	variable_t pv, sv, tv, fv;
	unsigned char nvariables;

	// 4-20 status
	bool loop_current_mode;
	char device_status;
	char extended_device_status_code;
	float UpperValue, LowerValue;
	float currentValue;
	char alert;
	char TF;
		
	void command0(char* data, unsigned char& size);
	void command1(char* data, unsigned char& size);
	//void command3(char* data, unsigned char& size);
	void command6(char* data, unsigned char& size);
	/*
	void command7(char* data, unsigned char& size);
	void command8(char* data, unsigned char& size);
	void command9(char* data, unsigned char& size);
	void command11(char* data, unsigned char& size);
	void command12(char* data, unsigned char& size);
	void command13(char* data, unsigned char& size);
	void command14(char* data, unsigned char& size);
	void command15(char* data, unsigned char& size);
	void command16(char* data, unsigned char& size);
	void command17(char* data, unsigned char& size);
	void command18(char* data, unsigned char& size);
	void command19(char* data, unsigned char& size);
	void command21(char* data, unsigned char& size);
	void command22(char* data, unsigned char& size);
	*/
};

#endif // __HART_PROTOCOL_AL