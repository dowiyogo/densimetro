#include "HARTProtocolPL.h"

///////////////////////////////////////
// Definitions

void HARTProtocolPL::RESETrequest()
{
	digitalWrite(HARTProtocolPL::pin_RESET, LOW);
	delay(10);
	digitalWrite(HARTProtocolPL::pin_RESET, HIGH);
}
//bool RESETconfirm();
void HARTProtocolPL::begin()
{
	SERIALX.begin(_UART_BAUD_RATE, _UART_CONF);
	SERIALX.setTimeout(20);
	pinMode(pin_RTS, OUTPUT);
	pinMode(pin_CD, INPUT);
	pinMode(pin_CTS, INPUT);
	pinMode(pin_RESET, OUTPUT);

	digitalWrite(pin_RTS, HIGH);
	digitalWrite(pin_RESET, HIGH);
}
void HARTProtocolPL::ENABLErequest()
{
	// !RTS used by AD5700 and DS8500
	digitalWrite(pin_RTS, LOW);
	//delay(tcon+tcdon);
}
bool HARTProtocolPL::ENABLEindicate()
{
  	return digitalRead(pin_CD) == HIGH;
}
bool HARTProtocolPL::ENABLEconfirm()
{
	return digitalRead(pin_CTS) == HIGH;
}
void HARTProtocolPL::DATArequest(char * data, int size)
{
	int wcount = 0;
	while (wcount < size)
		wcount += SERIALX.write(&data[wcount], size-wcount);
}
void HARTProtocolPL::DATAconfirm()
{
	SERIALX.flush();
	delay(tcoff + tcdoff);
	digitalWrite(pin_RTS, HIGH);
}
void HARTProtocolPL::DATAindicate(char * data, int & size)
{
	size = 0;
	while (digitalRead(pin_CD) == HIGH) {
		size += SERIALX.readBytes(&data[size], SERIALX.available());
	}
	size += SERIALX.readBytes(&data[size], SERIALX.available());
}
/*inline bool HARTProtocolPL::ERRORindicate(char status)
{

}*/
