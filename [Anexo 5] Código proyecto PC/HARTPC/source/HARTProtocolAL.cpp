#include "HARTProtocolAL.hpp"

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
	pv = {.value = 0f, .unit = 0, .classification = NOT_CLASSIFED, .code = 246};
	sv = {.value = 0f, .unit = 0, .classification = NOT_CLASSIFED, .code = 247};
	tv = {.value = 0f, .unit = 0, .classification = NOT_CLASSIFED, .code = 248};
	fv = {.value = 0f, .unit = 0, .classification = NOT_CLASSIFED, .code = 249};
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
	case(COMMAND2):
		command2(data, size);
		break;
	case(COMMAND3):
		command3(data, size);
		break;
	case(COMMAND6):
		command6(data, size);
		break;
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
	/*
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
	dll.getUniqueAddress(manufacturerID, deviceTypeID, uniqueID);

	data[0] = 0xFE;
	data[1] = manufacturerID;
	data[2] = deviceTypeID;
	data[3] = PREAMBLE_MIN_SIZE;
	data[4] = _UNIVERSAL_COMMAND_REVISION;
	data[5] = _DEVICE_REVISION[0];
	data[6] = _DEVICE_REVISION[1];
	data[7] = (_DEVICE_REVISION[2] & 0x1F) & (_PHYSICAL_SIGNAL_CODE << 6 & 0xE0);
	data[8] = _FLAG;
	data[9]  = uniqueID[0];
	data[10] = uniqueID[1];
	data[11] = uniqueID[2];
	data[12] = PREAMBLE_MIN_SIZE;
	data[13] = nvariables;
	data[14] = 0x00;
	data[15] = 0x00;
	data[16] = extended_device_status_code;

	size = 17;
}
void HARTProtocolAL::command1(char* data, unsigned char& size)
{
	data[0] = pv.unit;
	data[1] = pv.value & 0xFF;
	data[2] = (pv.value >> 8)  & 0xFF;
	data[3] = (pv.value >> 16) & 0xFF;
	data[4] = (pv.value >> 24) & 0xFF;
	size = 5;
}
void HARTProtocolAL::command2(char* data, unsigned char& size)
{
	float porcentageValue = (pv.value - LowerValue) / (UpperValue - LowerValue) * 100.0;
	data[0] = currentValue & 0xFF;
	data[1] = (currentValue >> 8)  & 0xFF;
	data[2] = (currentValue >> 16)  & 0xFF;
	data[3] = (currentValue >> 24)  & 0xFF;
	data[4] = porcentageValue & 0xDD;
	data[5] = (porcentageValue >> 8)  & 0xFF;
	data[6] = (porcentageValue >> 16)  & 0xFF;
	data[7] = (porcentageValue >> 24)  & 0xFF;
}
void HARTProtocolAL::command3(char* data, unsigned char& size)
{
	data[0] = currentValue & 0xFF;
	data[1] = (currentValue >> 8)  & 0xFF;
	data[2] = (currentValue >> 16)  & 0xFF;
	data[3] = (currentValue >> 24)  & 0xFF;
	size = 4;
	if (nvariables > 0) {
		data[4] = pv.unit;
		data[5] = pv.value & 0xFF;
		data[6] = (pv.value >> 8)  & 0xFF;
		data[7] = (pv.value >> 16) & 0xFF;
		data[8] = (pv.value >> 24) & 0xFF;
		size = 9;
	}
	else if (nvariables > 1) {
		data[9] = sv.unit;
		data[10] = sv.value & 0xFF;
		data[11] = (sv.value >> 8)  & 0xFF;
		data[12] = (sv.value >> 16) & 0xFF;
		data[13] = (sv.value >> 24) & 0xFF;
		size = 14;
	}
	else if (nvariables > 2) {
		data[14] = tv.unit;
		data[15] = tv.value & 0xFF;
		data[16] = (tv.value >> 8)  & 0xFF;
		data[17] = (tv.value >> 16) & 0xFF;
		data[18] = (tv.value >> 24) & 0xFF;
		size = 19;
	}
	else if (nvariables > 3) {
		data[19] = fv.unit;
		data[20] = fv.value & 0xFF;
		data[21] = (fv.value >> 8)  & 0xFF;
		data[22] = (fv.value >> 16) & 0xFF;
		data[23] = (fv.value >> 24) & 0xFF;
		size = 24;
	}
}
void HARTProtocolAL::command6(char* data, unsigned char& size)
{
	if (size < 2)
		return;

	dll.LOCAL_MANAGMENTrequest(SET_POLLING_ADDRESS, data, 1);
	loop_current_mode = data[1];
	size = 2;
}
void HARTProtocolAL::command7(char* data, unsigned char& size)
{
	data[0] = dll.getPollingAddress();
	data[1] = loop_current_mode;
	size = 2;
}
void HARTProtocolAL::command8(char* data, unsigned char& size)
{
	if (nvariables > 0) {
		data[0] = pv.classification;
		size = 1;
	}
	else if (nvariables > 1) {
		data[1] = pv.classification;
		size = 2;
	}
	else if (nvariables > 2) {
		data[2] = pv.classification;
		size = 3;
	}
	else if (nvariables > 3) {
		data[3] = pv.classification;
		size = 4;
	}
}
void HARTProtocolAL::command9(char* data, unsigned char& size)
{
	// Does not support other dynamic variables other than the standard 4
	if (size != 4) return;

	data[0] = extended_device_status_code;

	data[1] = pv.code;
	data[2] = pv.classification;
	data[3] = pv.unit;
	data[4] = pv.value & 0xFF;
	data[5] = (pv.value >> 8)  & 0xFF;
	data[6] = (pv.value >> 16) & 0xFF;
	data[7] = (pv.value >> 24) & 0xFF;
	data[8] = device_status;

	data[9]  = sv.code;
	data[10] = sv.classification;
	data[11] = sv.unit;
	data[12] = sv.value & 0xFF;
	data[13] = (sv.value >> 8)  & 0xFF;
	data[14] = (sv.value >> 16) & 0xFF;
	data[15] = (sv.value >> 24) & 0xFF;
	data[16] = device_status;

	data[17] = tv.code;
	data[18] = tv.classification;
	data[19] = tv.unit;
	data[20] = tv.value & 0xFF;
	data[21] = (tv.value >> 8)  & 0xFF;
	data[22] = (tv.value >> 16) & 0xFF;
	data[23] = (tv.value >> 24) & 0xFF;
	data[24] = device_status;

	data[25] = fv.code;
	data[26] = fv.classification;
	data[27] = fv.unit;
	data[28] = fv.value & 0xFF;
	data[29] = (fv.value >> 8)  & 0xFF;
	data[30] = (fv.value >> 16) & 0xFF;
	data[31] = (fv.value >> 24) & 0xFF;
	data[32] = device_status;

	size = 33;	
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