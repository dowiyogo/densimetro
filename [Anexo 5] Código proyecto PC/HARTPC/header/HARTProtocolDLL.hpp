#ifndef __HART_PROTOCOL_DLL
#define __HART_PROTOCOL_DLL

#include <vector>

#include <cstring>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "HARTProtocolDeclarations.hpp"
#include "HARTProtocolErrors.hpp"
#include "HARTProtocolPL.hpp"
#include "HARTProtocolPLStdio.hpp"


/******** HART DLL CLASS DEFINITION *******************************************
*
* HART Protocol Data Link Layer (DLL) implementation.
* 
* This layer shall carry a limited validation of the parameters
*/

class HARTProtocolDLL{
private:// Parameters
	
	static const enum frame_type INITIAL_FRAME_TYPE = ACK;
	static const unsigned char PREAMBLE_DEFAULT_SIZE = 5;
	static const char defaultPOLLINGADDR = 0;
	static const char defaultUNIQUEADDR[5];

public:	// Interface

	HARTProtocolDLL(HARTPLChannel channel);
	~HARTProtocolDLL();

	//** DLL SAPs
	//
	bool TRANSMITindicate(HARTCommand& command, char* data, unsigned char& dsize);
	bool TRANSMITresponse(HARTCommand& command, char* data, unsigned char dsize);

	void LOCAL_MANAGMENTrequest(HARTDLLService service, char* data, unsigned char dsize);
	void LOCAL_MANAGMENTconfirm(HARTDLLService& service, bool& status);

	// Needed for Application Layer
	void getPollingAddress(char& byte);
	void getUniqueAddress(char& n1, char& n2, char* n3);


private:

	/* HART Properties */
	
	HARTProtocolPL *PL;								// Physical Layer Socket
	unsigned char preambleSize;							// Preamble size used by the Slave
	char devicePollAddr;								// Device Polling Address
	char deviceUniqAddr[UNIQUE_ADDRESS_SIZE];					// Device Unique Address
	frame_type frameType;								// Slave actuating in Burst or Non-Burst modes

	/* Inner state of received and sended frames */
	char FBuffer[FRAME_MAX_SIZE];
	unsigned int FBsize;
	unsigned int FB_offset_position;
	unsigned int TB_offset_position;

	/* Transitional values for recieve and send */
	struct DLL_PDU_Metastack rcv_FHeaderInfo;
	HARTCommand     	 rcv_FCommand;
	unsigned char		 rcv_FDataSize;
	char*        		 rcv_FData;
	char            	 rcv_FCheckByte;

	HARTCommand 	snd_TCommand;
	unsigned char	snd_TDataSize;
	char*  		snd_TData;

	/* Inner LOCAL_MANAGMENT state */
	HARTDLLService last_service_requested;
	bool lsr_status;


	/* Private methods */

	inline bool getPreambleFB ();
	inline bool getStartMsgFB ();
	inline bool getDelimeterFB ();
	inline bool getExpansionFB ();
	inline bool getAddressFB ();
	inline bool getCommandFB ();
	inline bool getByteCountFB ();
	inline bool getDataFB ();
	inline bool getCheckSumFB ();
	inline bool verifyCheckSumFB ();

	inline bool setToBuffer (HARTCommand& command, char* data, unsigned char dsize);
	inline bool setPreambleTB ();
	inline bool setStartMsgTB ();
	inline bool setDelimeterTB ();
	inline bool setExpansionTB ();
	inline bool setAddressTB ();
	inline bool setCommandTB ();
	inline bool setByteCountTB ();
	inline bool setDataTB ();
	inline bool setCheckSumTB ();


	// Helpers
	inline void clearRcvHeaderInfo (struct DLL_PDU_Metastack& rcvStack);
	// sets frameType, physicalType, expansionSize, addressType from a byte
	inline void setRcvDelimeterValues (struct DLL_PDU_Metastack& rcvStack, char delimeterByte);
	// sets delimeter byte on sndStack
	inline char getDelimeterByte (frame_type f, physical_type p, unsigned char expansionSize, address_type a);
	inline void setRcvAddressBitsValues (struct DLL_PDU_Metastack& rcvStack, char addressByte);
	inline char maskFirstAddressByte (char first_byte, device_roll_type roll, frame_type type);
	inline char unmaskFirstAddressByte (char first_byte);
	inline char calculateCheckbyte (char* buffer, unsigned int bsize);
	inline void cleanRcvState ();
	inline void cleanSndState ();

	void setPollingAddress (char byte);
	void setUniqueAddress (char n1, char n2, char* n3);
};

#endif	// end '__HART_PROTOCOL_DLL' definition
