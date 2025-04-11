#ifndef __PACK_COMMUNICATION_H
#define __PACK_COMMUNICATION_H

#include <Boards.h>

class PackCommunication {
private:
	static const char SFLAG;
	static const char EFLAG;
	static const char ESCC;

private:
	HardwareSerial * ss;
	int byteT;
	bool flushing;

public:
	PackCommunication();
	~PackCommunication();

	bool begin(HardwareSerial & serial, bool _flushing=false);
	void send(char * bytes, int size_b);
	bool recv(char * bytes, int &size_b);
	bool recv_wait(char * bytes, int &size_b);
};

extern PackCommunication Paco;

#endif // #ifndef __PACK_COMMUNICATION_H