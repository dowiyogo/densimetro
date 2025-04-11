#include <PackCommunication.h>

PackCommunication Paco(Serial);

const int pin_RTS = 5;
const int pin_CD  = 6;
const int pin_CTS = 7;

const int tcoff  = 4;
const int tcdoff = 6;
const int tcon   = 4;
const int tcdon  = 6;
const int thold  = 19;

void setup ()
{
	Paco.begin(9600);
  	Serial3.begin(1200);

	pinMode(pin_RTS, INPUT);
	pinMode(pin_CD, OUTPUT);
	pinMode(pin_CTS, OUTPUT);
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
			millis0 = millis();

			// Demodulador
			millis0 = millis();
			digitalWrite(pin_CD, HIGH);
			delay(tcon);
			delay(tcdon);
			{
				Serial3.write(buff, size_send);
				Serial3.flush();
			}
			digitalWrite(pin_CD, LOW);
			delay(tcoff);
			delay(tcdoff);

			size_send = 0;

			Paco.send("t", 1);
			Paco.send(buff, size_send);
		}
		else {
			int millis1 = millis() - millis0;
			// Modulador
			digitalWrite(pin_CTS, HIGH);
			{
				size_read = 0;
				while (digitalRead(pin_RTS) == LOW)
					size_read += Serial3.readBytes(&buff[size_read], 20);
			}
			digitalWrite(pin_CTS, LOW);
			
			
			Paco.send("r", 1);
			Paco.send(buff, size_read);
			//Paco.send((char*)&millis1, 2);
		}
	}
}
