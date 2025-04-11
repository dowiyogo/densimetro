#include <PackCommunication.h>
#define SERIAL_BUFFER_SIZE  256
#define SERIAL3_BUFFER_SIZE 256

PackCommunication Paco(Serial);

const int pin_RTS = 7;
const int pin_CD  = 6;
const int pin_CTS = 5;

const int tcoff  = 4;
const int tcdoff = 6;
const int tcon   = 4;
const int tcdon  = 6;
const int thold  = 19;

void setup ()
{
	Paco.begin(9600);
  	Serial3.begin(1200, SERIAL_8O1);

	pinMode(pin_RTS, INPUT);
	pinMode(pin_CD, OUTPUT);
	//pinMode(pin_CTS, OUTPUT);
}

void loop ()
{
	char buff[512];
	int size_send = 0;
	int size_send_t;
	int size_read;

	digitalWrite(pin_CTS, LOW);
	digitalWrite(pin_CD, LOW);

	for (;;) {

		Paco.recv(buff, size_send_t);
		size_send = (size_send_t > 0)? size_send_t: size_send;
		int millis0;

		if (digitalRead(pin_RTS) == HIGH) {
			if (size_send == 0) continue;
			Paco.send("t", 1);
			Paco.send(buff, size_send);

			// Demodulador
			digitalWrite(pin_CD, HIGH);
			delay(tcon + tcdon);
			{
				int wcount = 0;
				while (wcount < size_send) {
					wcount += Serial3.write(&buff[wcount], size_send - wcount);
				}
				Serial3.flush();
			}
			delay(tcoff + tcdoff);
			digitalWrite(pin_CD, LOW);
			millis0 = millis();

			size_send = 0;
		}
		else {
			int millis1 = millis() - millis0;
			// Modulador

			//digitalWrite(pin_CTS, HIGH);
			size_read = 0;
			while (digitalRead(pin_RTS) == LOW)
				size_read += Serial3.readBytes(&buff[size_read], Serial3.available());
			size_read += Serial3.readBytes(&buff[size_read], Serial3.available());
			//digitalWrite(pin_CTS, LOW);
			
			Paco.send("r", 1);
			Paco.send(buff, size_read);
			//Paco.send((char*)&millis1, 2);
		}
	}
}
