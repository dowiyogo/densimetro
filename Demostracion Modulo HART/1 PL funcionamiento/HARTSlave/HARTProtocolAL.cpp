#include "HARTProtocolAL.h"

extern HARTProtocolDLL hart_dll;

HARTProtocolAL::HARTProtocolAL() :
	alert(0),
	nvariables(_NVARIABLES_DEFAULT),
	extended_device_status_code(0),
	UpperValue(10.0),
	LowerValue(0.0),
	loop_current_mode(false),
	currentValue(4.0),
	device_status(0),
	TF(0)
{
	pv = {.value = .0, .unit = (char)NOT_CLASSIFED, .classification = NOT_CLASSIFED, .code = 246};
	sv = {.value = .0, .unit = (char)NOT_CLASSIFED, .classification = NOT_CLASSIFED, .code = 247};
	tv = {.value = .0, .unit = (char)NOT_CLASSIFED, .classification = NOT_CLASSIFED, .code = 248};
	fv = {.value = .0, .unit = (char)NOT_CLASSIFED, .classification = NOT_CLASSIFED, .code = 249};
}
HARTProtocolAL::~HARTProtocolAL() {}

void HARTProtocolAL::setData(HARTCommand command, char* data, unsigned char& size)
{
	switch(command) {
	case(COMMAND0):
		command0(data, size);
		break;
	case(COMMAND1):
		command1(data, size);
		break;
	/*
	case(COMMAND2):
		command2(data, size);
		break;
	case(COMMAND3):
		command3(data, size);
		break;
	*/
	case(COMMAND6):
		command6(data, size);
		break;
	/*
	case(COMMAND7):
		command7(data, size);
		break;
	case(COMMAND8):
		command8(data, size);
		break;
	case(COMMAND9):
		command9(data, size);
		break;
	case(COMMAND11):
		command11(data, size);
		break;
	case(COMMAND12):
		command12(data, size);
		break;
	case(COMMAND13):
		command13(data, size);
		break;
	case(COMMAND14):
		command14(data, size);
		break;
	case(COMMAND15):
		command15(data, size);
		break;
	case(COMMAND16):
		command16(data, size);
		break;
	case(COMMAND17):
		command17(data, size);
		break;
	case(COMMAND18):
		command18(data, size);
		break;
	case(COMMAND19):
		command19(data, size);
		break;
	case(COMMAND20):
		command20(data, size);
		break;
	case(COMMAND21):
		command21(data, size);
		break;
	case(COMMAND22):
		command22(data, size);
		break;
	*/
	default:
		break;
	}
}

void HARTProtocolAL::command0(char* data, unsigned char& size)
{
	char manufacturerID, deviceTypeID, uniqueID[3];
	hart_dll.getUniqueAddress(manufacturerID, deviceTypeID, uniqueID);

	data[1]  = 0xFE;
	data[2]  = manufacturerID;
	data[3]  = deviceTypeID;
	data[4]  = PREAMBLE_MIN_SIZE;
	data[5]  = _UNIVERSAL_COMMAND_REVISION;
	data[6]  = _DEVICE_REVISION[0];
	data[7]  = _DEVICE_REVISION[1];
	data[8]  = (_DEVICE_REVISION[2] & 0x1F) & (_PHYSICAL_SIGNAL_CODE << 6 & 0xE0);
	data[9]  = _FLAG;
	data[10] = uniqueID[0];
	data[11] = uniqueID[1];
	data[12] = uniqueID[2];
	data[13] = PREAMBLE_MIN_SIZE;
	data[14] = nvariables;
	data[15] = 0x00;
	data[16] = 0x00;
	data[17] = extended_device_status_code;

	size = 18;
}
void HARTProtocolAL::command1(char* data, unsigned char& size)
{
	data[1] = pv.unit;
	data[2] = *((char*)&pv.value) & 0xFF;
	data[3] = (*((char*)&pv.value) >> 8)  & 0xFF;
	data[4] = (*((char*)&pv.value) >> 16) & 0xFF;
	data[5] = (*((char*)&pv.value) >> 24) & 0xFF;
	size = 6;
}
/*
void HARTProtocolAL::command2(char* data, unsigned char& size)
{
	float porcentageValue = (pv.value - LowerValue) / (UpperValue - LowerValue) * 100.0;
	data[1] = currentValue & 0xFF;
	data[2] = (currentValue >> 8)  & 0xFF;
	data[3] = (currentValue >> 16)  & 0xFF;
	data[4] = (currentValue >> 24)  & 0xFF;
	data[5] = porcentageValue & 0xDD;
	data[6] = (porcentageValue >> 8)  & 0xFF;
	data[7] = (porcentageValue >> 16)  & 0xFF;
	data[8] = (porcentageValue >> 24)  & 0xFF;
	size = 9;
}
void HARTProtocolAL::command3(char* data, unsigned char& size)
{
	data[1] = currentValue & 0xFF;
	data[2] = (currentValue >> 8)  & 0xFF;
	data[3] = (currentValue >> 16)  & 0xFF;
	data[4] = (currentValue >> 24)  & 0xFF;
	size = 5;
	if (nvariables > 0) {
		data[5] = pv.unit;
		data[6] = pv.value & 0xFF;
		data[7] = (pv.value >> 8)  & 0xFF;
		data[8] = (pv.value >> 16) & 0xFF;
		data[9] = (pv.value >> 24) & 0xFF;
		size = 10;
	}
	else if (nvariables > 1) {
		data[10] = sv.unit;
		data[11] = sv.value & 0xFF;
		data[12] = (sv.value >> 8)  & 0xFF;
		data[13] = (sv.value >> 16) & 0xFF;
		data[14] = (sv.value >> 24) & 0xFF;
		size = 15;
	}
	else if (nvariables > 2) {
		data[15] = tv.unit;
		data[16] = tv.value & 0xFF;
		data[17] = (tv.value >> 8)  & 0xFF;
		data[18] = (tv.value >> 16) & 0xFF;
		data[19] = (tv.value >> 24) & 0xFF;
		size = 20;
	}
	else if (nvariables > 3) {
		data[20] = fv.unit;
		data[21] = fv.value & 0xFF;
		data[22] = (fv.value >> 8)  & 0xFF;
		data[23] = (fv.value >> 16) & 0xFF;
		data[24] = (fv.value >> 24) & 0xFF;
		size = 25;
	}
}
*/
void HARTProtocolAL::command6(char* data, unsigned char& size)
{
	if (size < 2)
		return;

	hart_dll.LOCAL_MANAGMENTrequest(SET_POLLING_ADDRESS, data, 1);
	loop_current_mode = data[1];
	size = 2;
}
/*
void HARTProtocolAL::command7(char* data, unsigned char& size)
{
	data[1] = dll.getPollingAddress();
	data[2] = loop_current_mode;
	size = 3;
}
void HARTProtocolAL::command8(char* data, unsigned char& size)
{
	size = 1;
	if (nvariables > 0) {
		data[1] = pv.classification;
		size = 2;
	}
	if (nvariables > 1) {
		data[2] = pv.classification;
		size = 3;
	}
	if (nvariables > 2) {
		data[3] = pv.classification;
		size = 4;
	}
	if (nvariables > 3) {
		data[4] = pv.classification;
		size = 5;
	}
}
void HARTProtocolAL::command9(char* data, unsigned char& size)
{
	// Does not support other dynamic variables other than the standard 4
	if (size != 4) return;

	data[1] = extended_device_status_code;
	size = 2;
	if (nvariables > 0) {
		data[2] = pv.code;
		data[3] = pv.classification;
		data[4] = pv.unit;
		data[5] = pv.value & 0xFF;
		data[6] = (pv.value >> 8)  & 0xFF;
		data[7] = (pv.value >> 16) & 0xFF;
		data[8] = (pv.value >> 24) & 0xFF;
		data[9] = device_status;
		size = 10;
	}
	if (nvariables > 1) {
		data[10]  = sv.code;
		data[11] = sv.classification;
		data[12] = sv.unit;
		data[13] = sv.value & 0xFF;
		data[14] = (sv.value >> 8)  & 0xFF;
		data[15] = (sv.value >> 16) & 0xFF;
		data[16] = (sv.value >> 24) & 0xFF;
		data[17] = device_status;
		size = 18;
	}
	if (nvariables > 2) {
		data[18] = tv.code;
		data[19] = tv.classification;
		data[20] = tv.unit;
		data[21] = tv.value & 0xFF;
		data[22] = (tv.value >> 8)  & 0xFF;
		data[23] = (tv.value >> 16) & 0xFF;
		data[24] = (tv.value >> 24) & 0xFF;
		data[25] = device_status;
		size = 26;
	}
	if (nvariables > 3) {
		data[26] = fv.code;
		data[27] = fv.classification;
		data[28] = fv.unit;
		data[29] = fv.value & 0xFF;
		data[30] = (fv.value >> 8)  & 0xFF;
		data[31] = (fv.value >> 16) & 0xFF;
		data[32] = (fv.value >> 24) & 0xFF;
		data[33] = device_status;
		size = 34;
	}
}
void HARTProtocolAL::command11(char* data, unsigned char& size)
{
	char manufacturerID, deviceTypeID, uniqueID[3];
	dll.getUniqueAddress(manufacturerID, deviceTypeID, uniqueID);

	if (data[0] != manufacturerID && data[1] != deviceTypeID
		&& data[2] != uniqueID[0] && data[3] != uniqueID[1]
		&& data[4] != uniqueID[2]) {
		size = 0;
	} else {
		data[0] = 0;
		size = 1;
	}

}
void HARTProtocolAL::command12(char* data, unsigned char& size)
{
	for (int i=0; i<24; i++)
		data[i] = message[i];
	size = 24;
}
void HARTProtocolAL::command13(char* data, unsigned char& size)
{
	for (int i=0; i<6; i++)
		data[i] = tag[i];
	for (int i=0; i<11; i++)
		data[i+6] = descriptor[i];
	for (int i=0; i<3; i++)
		data[i+18] = date[i];
	size = 21;
}
void HARTProtocolAL::command14(char* data, unsigned char& size)
{
	for (int i=0; i<3; i++)
		data[i] = 0;
	data[3] = 0;
	for (int i=0; i<3; i++) {
		data[i*4+4] = 0x7F;
		data[i*4+5] = 0xA0;
		data[i*4+6] = 0x00;
		data[i*4+7] = 0x00;
	}
	size = 16;
}
void HARTProtocolAL::command15(char* data, unsigned char& size)
{
	float porcentageValue = (pv.value - LowerValue) / (UpperValue - LowerValue) * 100.0;

	data[0] = alert;
	data[1] = TF;
	data[2] = 0;
	data[17] = _FLAG;
	size = 18;
}
void HARTProtocolAL::command16(char* data, unsigned char& size)
{}
void HARTProtocolAL::command17(char* data, unsigned char& size)
{
	for (int i=0; i<24; i++)
		message[i] = data[i];
	size = 24;
}
void HARTProtocolAL::command18(char* data, unsigned char& size)
{
	for (int i=0; i<6; i++)
		tag[i] = data[i];
	for (int i=0; i<11; i++)
		descriptor[i] = data[i+6];
	for (int i=0; i<3; i++)
		data[i+18] = date[i];
	size = 21;
}
void HARTProtocolAL::command19(char* data, unsigned char& size)
{}
void HARTProtocolAL::command20(char* data, unsigned char& size)
{}
void HARTProtocolAL::command21(char* data, unsigned char& size)
{}
void HARTProtocolAL::command22(char* data, unsigned char& size)
{}
*/