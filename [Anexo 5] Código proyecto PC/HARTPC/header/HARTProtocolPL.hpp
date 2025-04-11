#ifndef __HART_PROTOCOL_PL
#define __HART_PROTOCOL_PL

/*
	HART's protocol Physical Layer Service Access Points (SAPs).

 	
 	What it is this classs ?

	This Access Points consists in the RESET, ENABLE, DATA and ERROR services. As any HART service it, each one can be composed up to four sequences: request, indicate, response and confirm. The pairs request and confirm are used on the emitter side; the pairs indicate and response are used in the receiver side.

	The Physical Layer SAPs are used to communicate with the physical device that performs the moduletion, the modem.
	It is noticeable that in all this interactions there is no response involved in the sequences of communication.

	This is a virtual class made to deside the modem used.


	How it is this class ?

	This class, as all HART_PROTOCOL API, is designed to mimic the HART specifiecations. The arguments are the same as in the specification to make it easier to understand. Request is a emitter that return 'void' and confirm returns a boolean value that says the 'true' or 'false' value of the request made. Indicate return 'true' or 'false' for a state in connection and response return void as it only is responsive to a initial request and they only carry the message of that initial request signal.


	* The commented arguemntes are there because so says the specification, but aren't mentioned in the description of the Service nor are useful.
	
*/

#include "HARTProtocolErrors.hpp"


class HARTProtocolPL {
public:
	// Symbols
	static const bool TPREPARED  = true;		// STATE : prepare to transmit
	static const bool TTERMINATE = false;		// STATE : transmission to terminate

	virtual ~HARTProtocolPL() {};

	// SAPs
	virtual void RESETrequest () =0;
	virtual bool RESETconfirm () =0;

	virtual void ENABLErequest (bool state) =0;
	virtual bool ENABLEindicate(bool& state) =0;			// Maybe a time turn-on and turn-off delay to be used
	virtual bool ENABLEconfirm (bool& state) =0;

	virtual void DATArequest (char* frameData, unsigned int fDsize) =0;
	virtual bool DATAconfirm (/*vector<usigned char>& data*/) =0;
	virtual void DATAindicate (char* frameData, unsigned int& fDsize) =0;

	virtual HART_Protocol_Errors_types& ERRORindicate (bool& status/*, vector<usigned char>& data*/) =0;

	/*  PHYSICAL LAYER SAPs

		RESET
		Allow to request a reset to the modem. Then the device may return a confirm().
	
		ENABLE
		Allow to know the state of the communication on the modem.
		Mapping for each method:
			request  -> Request To Send (RTS) signal
			indicate -> Carrier Detect (CD) signal
			confirm  -> Clear To Send (CTS) signal

		So for a emitter, the sequence is: request and confirm. With the confirm() method the message is possible or not to send.

		ENABLEindicate returns if it has (true) or has not (false) received a request
		ENABLEconfirm returns if it's avaible to receive (true) or not (false) data
	
    	DATA
    	Does the transfer of the data to the modem. vector<usigned char>& data is the raw bytes that conforms a frame in the HART protocol. So, the data vector is composed of the start byte, headers, data and end byte for the frame. No corroboration of a well-structured packes is made.

    	DATAindicate returns if has received (true) or not (false) some data
    	DATAconfirm returns if the modem has send all the data (true) or not (false) to the channel
   	
		ERROR
		Indicates to the API any error detected by the program on the modem.
	*/

protected: // Properties
	bool state;
	HART_Protocol_Errors_types error;

};

#endif