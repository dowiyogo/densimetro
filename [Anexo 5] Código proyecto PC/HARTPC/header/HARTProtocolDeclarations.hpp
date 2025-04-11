#ifndef _HART_PROTOCOL_DECLARATIONS
#define _HART_PROTOCOL_DECLARATIONS


/******** APPLICATION LAYER PARAMETERS *********/
const char _UNIVERSAL_COMMAND_REVISION = 0;					// Universal Command Revision version
const unsigned char _NVARIABLES_DEFAULT = 1;

/******** DATA LINK LAYER PARAMTERS ************/
// General
const char _DEVICE_ID_SIZE = 3;							// Device ID bytes length
const char _DEVICE_MANUF_ID = 0x00;						// Default manufacturer ID
const char _DEVICE_TYPE = 0x00;							// Default device type
const char _DEVICE_REVISION[3] = {0,0,0};					// Default 3 bytes device revision
const char _PHYSICAL_SIGNAL_CODE = 0x00;					// Common tables, Table 10. Bell202 current
const char _FLAG = 0x01;							// Common tables, Table 11. Multi-Sensor Field Device


/******** DLL FRAMING CONSTANTS ****************/

const unsigned char PREAMBLE_MIN_SIZE      = 5;					// Minimun Preamble Length
const unsigned char PREAMBLE_MAX_SIZE      = 20;				// !For FSK 5 is recommended. Number of bytes to used previous delimiter byte
const          char PREAMBLE_DEFAULT_VALUE = 0xFF;				// !Established by protocol (it's a byte => char (not unsigned))
const unsigned char START_OF_MSG_SIZE      = 1;					// !Established by protocol for FSK and RS485
const unsigned char DELIMETER_SIZE         = 1;					// !Established by protocol. Delimeter Byte
const unsigned char POLLING_ADDRESS_SIZE   = 1;					// !Established by protocol. N° of bytes for Polling address
const unsigned char UNIQUE_ADDRESS_SIZE    = 5;					// !Established by protocol. N° of bytes for Unique address of device
const unsigned char EXPANSION_MAX_SIZE     = 4;					// !Established by protocol. Expansion max. number of bytes per frame
const unsigned char EXPANSION_DEFAULT_SIZE = 0;					// Default Expansion bytes size (if not 0 goto HARTDLL::getDelimeterFB)
const unsigned char COMMAND_SIZE           = 1;					// !Established by protocol. Command sended byte
const unsigned char BYTE_COUNT_SIZE        = 1;					// !Established by protocol. Byte Count for data size byte
const unsigned char DATA_MAX_NBYTES        = 255;				// !Established by protocol. Max. number of bytes allowed in [data] per frame
const unsigned char DATA_ERROR_BYTES       = 2;					// !Error bytes for Slave response
const unsigned char CHECK_BYTE_SIZE        = 1;					// !Established by protocol. Xor of all bytes
const unsigned int  FRAME_MAX_SIZE         = ((unsigned int)PREAMBLE_MAX_SIZE)
	+START_OF_MSG_SIZE+DELIMETER_SIZE+UNIQUE_ADDRESS_SIZE+EXPANSION_MAX_SIZE
	+COMMAND_SIZE+BYTE_COUNT_SIZE+DATA_MAX_NBYTES+CHECK_BYTE_SIZE;


/******** LIBRARY DEFINITIONS ******************/

enum address_type{POLLING_ADDRESS=0, UNIQUE_ADDRESS=1};				// Type of address in use (Polling : 1byte ; Unique : 5bytes)

enum HARTCommand{COMMAND0=0, COMMAND1=1, COMMAND3=3,
	COMMAND59=59, COMMAND255=255};
										// HART commands (0 implemented)

enum device_roll_type{SMASTER=0, PMASTER=1, SLAVE=2};				// Device roll in communication
enum stack_state{STACK_CLEANED, STACKING, STACK_DONE};				// State of stack push (Are all stack variables setted?)
enum frame_type{NO_BACK, BACK=1, STX=2, ACK=6, NO_FT};				// BACK: Burst-Mode; STX: Master-TX; ACK: Slave-TX ; NO_BACK: either ACK or STX
enum physical_type{FSK=0, OTHER_MODULATION=1};					// Physical modulation (e.x., PSK=1). No other than FSK supported

enum HARTDLLService{SET_RETRY_LIMIT, SET_MASTER_ADDRESS,
CAPTURE_BURST_FRAMES, IGNORE_BURST_FRAMES, IGNORE_OTHER_MASTER_FRAMES,
										// Table 3 services (MASTER)
SET_UNIQUE_ADDRESS, SET_POLLING_ADDRESS, SET_PREAMBLE
										// Table 4 services (SLAVE)
										// Not implemented : SET_CAPTURE_ADDRESS, DISABLE_CAPTURE,
										//                   ENABLE_BURST_MODE, DISABLE_CAPTURE_MODE
};
enum HARTClassification{
	NOT_CLASSIFED = 0, TEMPERATURE=64, PRESSURE, VOL_FLOW, VELOCITY,
	VOLUME, LENGTH, TIME, MASS, MASS_FLOW, MASS_PER_VOL, VISCOSITY,
	ANG_VEL, AREA, WORK, FORCE, POWER, FREQ, ANALYTICAL, CAPACITANCE,
	EMF, CURRENT, RESISTANCE, ANGLE, CONDUCTANCE, VOL_PER_VOL, VOL_PER_MASS,
	CONCENTRATION, VALVE_ACTUATOR, LEVEL, VORTEX_FLOW, MAG_FLOW, CORIOLIS_FLOW
};

enum HARTPLChannel{PL_CHANNEL_STDIO, PL_CHANNEL_SOCKET
										// Physical Layer APIs implemented
};

// HART DLL metastack
struct DLL_PDU_Metastack{
	frame_type       frameType;						// - BACK, STX or ACK
	physical_type    physicalType;						// - FSK, OTHER (PSK, ...)
	unsigned char    expansionSize;						// - Expansion byte size (0, 1, 2 or 3)
	address_type     addressType;						// - Address type (Polling or Unique)
	device_roll_type communicatingWith;					// Communicating with/from Primary Master (PMASTER) or Secondary Master (SMASTER)
	frame_type       communicationType;					// Address' information about frame type (BACK or not BACK)
	int              frameLength;						// Length of the whole frame (preamble not included)
	stack_state      state;
};
struct variable_t{
	float value;
	char unit;
	HARTClassification classification;
	char code;
};


#endif
