#include "HARTProtocolDLL.hpp"

extern HART_Protocol_Errors HARTProtocolErrors;

const char HARTProtocolDLL::defaultUNIQUEADDR[] = {0x00,0x00,0x00};

/************** class HARTProtocolDLL *****/

HARTProtocolDLL::HARTProtocolDLL(HARTPLChannel channel)
{
	devicePollAddr = 0x00;
	deviceUniqAddr[0] = 0x00;
	deviceUniqAddr[1] = 0x00;
	deviceUniqAddr[2] = 0x00;
	deviceUniqAddr[3] = 0x00;
	deviceUniqAddr[4] = 0x00;

	preambleSize = PREAMBLE_DEFAULT_SIZE;
	frameType    = ACK;

	FB_offset_position = 0;	

	if (channel == PL_CHANNEL_STDIO)
		PL = new HARTProtocolPLStdio();
}
HARTProtocolDLL::~HARTProtocolDLL()
{
	delete PL;
}

/******************************************************** PUBLIC METHODS ******/

void HARTProtocolDLL::getPollingAddress (char& byte){
	// Polling address defined only by it's first 6 bits
	byte = unmaskFirstAddressByte(devicePollAddr);
}
void HARTProtocolDLL::getUniqueAddress(char& n1, char& n2, char* n3)
{
	n1    = unmaskFirstAddressByte(deviceUniqAddr[0]);
	n2    = deviceUniqAddr[1];
	n3[0] = deviceUniqAddr[2];
	n3[1] = deviceUniqAddr[3];
	n3[2] = deviceUniqAddr[4];
}

bool HARTProtocolDLL::TRANSMITindicate (HARTCommand& command, char* data, unsigned char& dsize)
{
	bool pl_state = false;
	FBsize = 0;
	cleanRcvState();
	HARTProtocolErrors.clear();

	if (!PL->ENABLEindicate(pl_state)) 	return false;
	PL->DATAindicate(FBuffer, FBsize);
	
	if (!getPreambleFB())	return false;
	if (!getStartMsgFB())	return false;
	if (!getDelimeterFB())	return false;
	if (!getAddressFB())	return false;
	if (!getExpansionFB())	return false;
	if (!getCommandFB())	return false;
	if (!getByteCountFB())	return false;
	if (!getDataFB())	return false;
	if (!getCheckSumFB())	return false;
	if (!verifyCheckSumFB())return false;

	command = rcv_FCommand;
	dsize   = rcv_FDataSize;
	for(int i=0;i<dsize;i++){ data[i] = rcv_FData[i]; }

	return true;
}
bool HARTProtocolDLL::TRANSMITresponse (HARTCommand& command, char* data, unsigned char dsize)
{
	cleanSndState();
	HARTProtocolErrors.clear();

	setToBuffer(command, data, dsize);
	if (!setPreambleTB())	return false;
	if (!setStartMsgTB())	return false;
	if (!setDelimeterTB())	return false;
	if (!setAddressTB())	return false;
	if (!setExpansionTB())	return false;
	if (!setCommandTB())	return false;
	if (!setByteCountTB())	return false;
	if (!setDataTB())	return false;
	if (!setCheckSumTB())	return false;

	bool pl_state = true;
	PL->ENABLErequest(pl_state);
	PL->ENABLEconfirm(pl_state);
	if (!pl_state) 	return false;
	
	PL->DATArequest(FBuffer, FB_offset_position);
	if (!PL->DATAconfirm())	return false;

	return true;
}


void HARTProtocolDLL::LOCAL_MANAGMENTrequest (HARTDLLService service, char* data, unsigned char dsize)
{
	last_service_requested = service;
	lsr_status = false;

	switch (service) {
		case (SET_UNIQUE_ADDRESS) : {
			if (dsize == UNIQUE_ADDRESS_SIZE) {
				lsr_status = false;
			} else {
				deviceUniqAddr[0] = (data[0] & 0x3F) | (0xC0 & deviceUniqAddr[0]);
				deviceUniqAddr[1] = data[1];
				deviceUniqAddr[2] = data[2];
				deviceUniqAddr[3] = data[3];
				deviceUniqAddr[4] = data[4];
				lsr_status = true;
			}
			break;
		}
		case (SET_POLLING_ADDRESS) : {
			if (dsize == UNIQUE_ADDRESS_SIZE) {
				lsr_status = false;
			} else {
				devicePollAddr = (data[0] & 0x3F) | (0xC0 & devicePollAddr);
				lsr_status = true;
			}
			break;
		}
		case (SET_PREAMBLE) : {
			if (dsize != 1
			&& data[0] < PREAMBLE_MIN_SIZE
			&& data[0] > PREAMBLE_MAX_SIZE) {
				preambleSize = PREAMBLE_DEFAULT_VALUE;
				lsr_status = false;
			} else {
				preambleSize = data[0];
				lsr_status = true;
			}
			break;
		}
		default : {
			break;
		}
	}
}
void HARTProtocolDLL::LOCAL_MANAGMENTconfirm (HARTDLLService& service, bool& status)
{
	service = last_service_requested;
	status = lsr_status;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


// Getters from Received Buffer

inline bool HARTProtocolDLL::getPreambleFB ()
{
	if (HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + preambleSize) {
		HARTProtocolErrors.set(PREAMBLE_BYTES_NOT_RECEIVED);
		return false;
	}

	for (unsigned int i=0; i<preambleSize; i++) {
		if (FBuffer[FB_offset_position+i] != PREAMBLE_DEFAULT_VALUE) {
			HARTProtocolErrors.set(PREAMBLE_VALUE_ERROR);
			return false;
		}
	}
	FB_offset_position += preambleSize;
	
	return true;
}
inline bool HARTProtocolDLL::getStartMsgFB ()
{
	if (HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + START_OF_MSG_SIZE) {
		HARTProtocolErrors.set(START_MESSAGE_NOT_RECEIVED);
		return false;
	}

	for (unsigned int i=0; i<START_OF_MSG_SIZE; i++) {
		if (FBuffer[FB_offset_position+i] != PREAMBLE_DEFAULT_VALUE) {
			HARTProtocolErrors.set(START_MESSAGE_VALUE_ERROR);
			return false;
		}
	}
	FB_offset_position += START_OF_MSG_SIZE;
	
	return true;
}
inline bool HARTProtocolDLL::getDelimeterFB ()
{
	if (HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + DELIMETER_SIZE) {
		HARTProtocolErrors.set(DELIMETER_BYTES_NOT_RECEIVED);
		return false;
	}

	unsigned char delimeterByte = FBuffer[FB_offset_position];
	setRcvDelimeterValues(rcv_FHeaderInfo, delimeterByte);

	if (rcv_FHeaderInfo.expansionSize !=  EXPANSION_DEFAULT_SIZE) {
		HARTProtocolErrors.set(FRAME_NOT_ZERO_EXPANS);
		return false;
	}
	else if(rcv_FHeaderInfo.physicalType != FSK) {
		HARTProtocolErrors.set(FRAME_NOT_FSK);
		return false;
	}

	FB_offset_position += DELIMETER_SIZE;

	return true;
}
inline bool HARTProtocolDLL::getAddressFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR) return false;
	if (rcv_FHeaderInfo.addressType == POLLING_ADDRESS) {
		if (FBsize< FB_offset_position + POLLING_ADDRESS_SIZE) {
			HARTProtocolErrors.set(PADDRESS_NOT_RECEIVED);
			return false;
		}
		if (unmaskFirstAddressByte(FBuffer[FB_offset_position]) != devicePollAddr) {
			HARTProtocolErrors.set(RCV_ADDRESS_NOT_MATCH);
			return false;
		}

		FB_offset_position += POLLING_ADDRESS_SIZE;
	}
	else {
		if (FBsize < FB_offset_position + UNIQUE_ADDRESS_SIZE) {
			HARTProtocolErrors.set(UADDRESS_NOT_RECEIVED);
			return false;
		}
		if (unmaskFirstAddressByte(FBuffer[FB_offset_position]) != deviceUniqAddr[0]) {
			HARTProtocolErrors.set(RCV_ADDRESS_NOT_MATCH);
			return false;
		}
		for (unsigned int i=1; i<UNIQUE_ADDRESS_SIZE; i++) {
			if (FBuffer[FB_offset_position+i] != deviceUniqAddr[i]) {
				HARTProtocolErrors.set(RCV_ADDRESS_NOT_MATCH);
				return false;
			}
		}

		FB_offset_position += UNIQUE_ADDRESS_SIZE;
	}

	return true;	
}
inline bool HARTProtocolDLL::getExpansionFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + rcv_FHeaderInfo.expansionSize) {
		HARTProtocolErrors.set(EXPANSION_NOT_RECEIVED);
		return false;
	}

	/*
	for (int i=0; i<rcv_FHeaderInfo.expansionSize; i++) {
		if () {}
	}
	*/

	FB_offset_position += rcv_FHeaderInfo.expansionSize;

	return true;
}
inline bool HARTProtocolDLL::getCommandFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + COMMAND_SIZE) {
		HARTProtocolErrors.set(COMMAND_NOT_RECEIVED);
		return false;
	}

	rcv_FCommand = (enum HARTCommand)FBuffer[FB_offset_position];
	FB_offset_position += COMMAND_SIZE;

	if(rcv_FHeaderInfo.addressType == POLLING_ADDRESS && rcv_FCommand != COMMAND0) {
		// Do not allow to responde to polling if its not command 0
		HARTProtocolErrors.set(RCV_ADDRESS_NOT_MATCH);
		return false;
	}

	return true;
}
inline bool HARTProtocolDLL::getByteCountFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + BYTE_COUNT_SIZE) {
		HARTProtocolErrors.set(BYTE_COUNT_NOT_RECEIVED);
		return false;
	}

	rcv_FDataSize = FBuffer[FB_offset_position];
	FB_offset_position += BYTE_COUNT_SIZE;

	return true;
}
inline bool HARTProtocolDLL::getDataFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + rcv_FDataSize) {
		HARTProtocolErrors.set(DATA_NOT_RECEIVED);
		return false;
	}

	rcv_FData = &FBuffer[FB_offset_position];
	FB_offset_position += rcv_FDataSize;

	return true;
}
inline bool HARTProtocolDLL::getCheckSumFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (FBsize < FB_offset_position + CHECK_BYTE_SIZE) {
		HARTProtocolErrors.set(CHECK_BYTE_NOT_RECEIVED);
		return false;
	}

	rcv_FCheckByte = FBuffer[FB_offset_position];
	FB_offset_position += CHECK_BYTE_SIZE;

	return true;
}
inline bool HARTProtocolDLL::verifyCheckSumFB ()
{
	if(HARTProtocolErrors.get() != NO_ERROR)
		return false;
	if (rcv_FCheckByte != calculateCheckbyte(FBuffer, FB_offset_position)) {
		HARTProtocolErrors.set(RCV_BAD_CHECK_BYTE);
		return false;
	}

	return true;
}

/* Setters to to-be-sended Buffer */

inline bool HARTProtocolDLL::setToBuffer (HARTCommand& command, char* data, unsigned char dsize)
{
	snd_TCommand  = command;
	snd_TDataSize = (unsigned char) dsize;
	snd_TData     = data;
	TB_offset_position = 0;

	return true;
}
inline bool HARTProtocolDLL::setPreambleTB ()
{
	for (unsigned int i=0; i<preambleSize; i++) {
		FBuffer[TB_offset_position+i] = PREAMBLE_DEFAULT_VALUE;
	}
	TB_offset_position += preambleSize;
	
	return true;
}
inline bool HARTProtocolDLL::setStartMsgTB ()
{
	for (unsigned int i=0; i<START_OF_MSG_SIZE; i++) {
		FBuffer[TB_offset_position+i] = PREAMBLE_DEFAULT_VALUE;
	}
	TB_offset_position += START_OF_MSG_SIZE;
	
	return true;
}
inline bool HARTProtocolDLL::setDelimeterTB ()
{
	char delimeter = getDelimeterByte(rcv_FHeaderInfo.frameType,
		rcv_FHeaderInfo.physicalType, rcv_FHeaderInfo.expansionSize, rcv_FHeaderInfo.addressType);
	FBuffer[TB_offset_position] = delimeter;
	TB_offset_position += DELIMETER_SIZE;

	return true;
}
inline bool HARTProtocolDLL::setAddressTB ()
{
	if (rcv_FHeaderInfo.addressType == POLLING_ADDRESS) {
		FBuffer[TB_offset_position] = devicePollAddr;
		TB_offset_position += POLLING_ADDRESS_SIZE;
	}
	else {
		for (unsigned int i=0; i<UNIQUE_ADDRESS_SIZE; i++) {
			FBuffer[TB_offset_position+i] = deviceUniqAddr[i];
		}
		TB_offset_position += UNIQUE_ADDRESS_SIZE;
	}

	return true;	
}
inline bool HARTProtocolDLL::setExpansionTB ()
{
	for (int i=0; i<rcv_FHeaderInfo.expansionSize; i++) {
		FBuffer[TB_offset_position+i] = 0x00;					// ?????
	}
	TB_offset_position += rcv_FHeaderInfo.expansionSize;

	return true;
}
inline bool HARTProtocolDLL::setCommandTB ()
{
	FBuffer[TB_offset_position] = (char) snd_TCommand;
	TB_offset_position += COMMAND_SIZE;

	return true;
}
inline bool HARTProtocolDLL::setByteCountTB ()
{
	FBuffer[TB_offset_position] = DATA_ERROR_BYTES + snd_TDataSize;
	TB_offset_position += BYTE_COUNT_SIZE;

	return true;
}
inline bool HARTProtocolDLL::setDataTB ()
{
	/* 2-bytes error for responses */
	if (HARTProtocolErrors.get() != NO_ERROR)
		FBuffer[TB_offset_position] = 0x80;		// Fill with Command Summary Specification
	else
		FBuffer[TB_offset_position] = 0x00;

	FBuffer[TB_offset_position]  |= 0x00;
	FBuffer[TB_offset_position+1] = 0x00;
	TB_offset_position += 2;

	for (unsigned int i=0; i<snd_TDataSize; i++) {
		FBuffer[TB_offset_position+i] = snd_TData[i];
	}
	TB_offset_position += snd_TDataSize;

	return true;
}
inline bool HARTProtocolDLL::setCheckSumTB ()
{
	FBuffer[TB_offset_position] = calculateCheckbyte(FBuffer, TB_offset_position);
	TB_offset_position += CHECK_BYTE_SIZE;

	return true;
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/********************************************************** PRIVATE METHODS **/


inline void HARTProtocolDLL::clearRcvHeaderInfo (struct DLL_PDU_Metastack& rcvHInfo)
{
	rcvHInfo.frameType         = NO_FT;
	rcvHInfo.physicalType      = FSK;
	rcvHInfo.addressType       = UNIQUE_ADDRESS;
	rcvHInfo.expansionSize     = 0;
	rcvHInfo.communicatingWith = PMASTER;
	rcvHInfo.frameLength       = 0;
	rcvHInfo.state             = STACK_CLEANED;
}
inline char HARTProtocolDLL::getDelimeterByte (frame_type f, physical_type p, unsigned char expansionSize, address_type a)
{
	char delimeterByte = 0x00;
	delimeterByte |= (a==UNIQUE_ADDRESS)? 0x80: 0x00;
	delimeterByte |= 0x60 &(expansionSize<<5);
	delimeterByte |= (p == FSK)? 0x00: 0x04;			// PSK otherwise
	delimeterByte |= 0x07 &(f<<0);

	return delimeterByte;
}
inline void HARTProtocolDLL::setRcvDelimeterValues (struct DLL_PDU_Metastack& rcvHInfo, char delimeterByte)
{
	rcvHInfo.frameType     = ((delimeterByte &0x07)==1)? BACK:
							 ((delimeterByte &0x07)==2)? STX:
							 ((delimeterByte &0x07)==6)? ACK: NO_FT;
	rcvHInfo.physicalType  = (((delimeterByte&0x18)>>3)==0)? FSK: OTHER_MODULATION;
	rcvHInfo.expansionSize = (delimeterByte  &0x60)>>5;
	rcvHInfo.addressType   = (((delimeterByte&0x80)>>7)==0)? POLLING_ADDRESS: UNIQUE_ADDRESS;
}
inline void HARTProtocolDLL::setRcvAddressBitsValues (struct DLL_PDU_Metastack& rcvHInfo, char addressByte)
{
	rcvHInfo.communicatingWith = (addressByte &0x80)? PMASTER: SMASTER;
	rcvHInfo.communicationType = (addressByte &0x40)? BACK: NO_BACK;;
}
inline char HARTProtocolDLL::calculateCheckbyte (char* buffer, unsigned int bsize)
{
	char checksum = 0x00;
	for (unsigned int i=preambleSize+START_OF_MSG_SIZE; i<bsize-1; i++) {
		checksum ^= buffer[i];
	}
	//printf("\n--debug: checksum == %02x\n", checksum);
	return checksum;
}
inline char HARTProtocolDLL::maskFirstAddressByte (char first_byte, device_roll_type roll, frame_type type)
{
	char addressByte = first_byte & 0x3F;
	addressByte |= (roll == PMASTER)? 0x80: 0x00;
	addressByte |= (type == BACK)?    0x40: 0x00;

	return addressByte;
}
inline char HARTProtocolDLL::unmaskFirstAddressByte (char first_byte)
{
	return first_byte & 0x3F;
}

inline void HARTProtocolDLL::cleanRcvState ()
{
	clearRcvHeaderInfo(rcv_FHeaderInfo);
	rcv_FCommand   = COMMAND0;
	rcv_FDataSize  = 0;
	rcv_FData      = NULL;
	rcv_FCheckByte = 0;
	FBsize = 0;
	FB_offset_position = 0;
	HARTProtocolErrors.set(NO_ERROR);
}
inline void HARTProtocolDLL::cleanSndState ()
{
	snd_TCommand  = COMMAND0;
	snd_TDataSize = 0;
	snd_TData     = NULL;
	TB_offset_position = 0;
}
void HARTProtocolDLL::setPollingAddress (char byte)
{
	// Polling address defined only by it's first 6 bits
	devicePollAddr = unmaskFirstAddressByte(byte);
}
void HARTProtocolDLL::setUniqueAddress (char n1, char n2, char* n3)
{
	deviceUniqAddr[0] = (n1 & 0xC0) | (0x80 | 0x00);
	deviceUniqAddr[1] = n2;
	deviceUniqAddr[2] = n3[0];
	deviceUniqAddr[3] = n3[1];
	deviceUniqAddr[4] = n3[2];
}