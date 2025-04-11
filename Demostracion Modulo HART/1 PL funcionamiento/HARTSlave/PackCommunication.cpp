#include "PackCommunication.h"

PackCommunication Paco;

const char PackCommunication::SFLAG = 0x30;
const char PackCommunication::EFLAG = 0x31;
const char PackCommunication::ESCC  = 0x32;

PackCommunication::PackCommunication() :
	ss      (nullptr),
	byteT   (1000000/9600),
	flushing(false) {}
PackCommunication::~PackCommunication(){}

bool PackCommunication::begin(HardwareSerial &serial, bool _flushing)
{
	ss       = &serial;
	byteT    = 1000000/9600;
	flushing = _flushing;
	return ss;
}

void PackCommunication::send(char *bytes, int size_b)
{
	if (size_b < 1)	return;

	ss->write(SFLAG);
	//ss->flush();
	char c;
	for (int i=0;i<size_b;i++) {
		c = (bytes[i]);
		if (c == SFLAG || c == EFLAG || c == ESCC) {
			ss->write(ESCC);
			//ss->flush();
		}
		ss->write(c);
		//ss->flush();
	}
	ss->write(EFLAG);
	if (flushing)
		ss->flush();
}

bool PackCommunication::recv(char *bytes, int &size_b)
{
	size_b = 0;
	if (ss->available() < 1) return false;

	char c;
	if (ss->readBytes(&c, 1) < 1 || c != SFLAG)	return false;
	do {
		while (ss->available() < 1) delayMicroseconds(byteT);
		ss->readBytes(&c, 1);
		if (c == EFLAG) break;
		if (c == ESCC) {
			while (ss->available() < 1) delayMicroseconds(byteT);
			ss->readBytes(&c, 1);
		}
		bytes[size_b++] = c;
	} while (1);

	return true;
}

bool PackCommunication::recv_wait(char *bytes, int &size_b)
{
	while (!recv(bytes, size_b)) {
		delayMicroseconds(byteT);
	} 
	return true;
}
