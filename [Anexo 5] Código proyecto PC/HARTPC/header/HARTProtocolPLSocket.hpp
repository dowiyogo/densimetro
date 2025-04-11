#ifndef __HART_PROTOCOL_PL_SOCKET
#define __HART_PROTOCOL_PL_SOCKET

#include <vector>
using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "HARTProtocolDeclarations.hpp"
#include "HARTProtocolErrors.hpp"
#include "HARTProtocolPL.hpp"


class HARTProtocolPLSocket{
private:
    static const int SOCKET_BUFFER_SIZE = FRAME_MAX_SIZE*2+1;
    static const char *const socketIP;                  // server local socket
    static const int socketPort = 11589;                // local connection port

    int socketfp;                                       // host socket file descriptor
    int clintSockfp;                                    // client socket file descriptor
    struct addrinfo* res;                               // address info of socket

    char state;                                         // 1 -> connetion accepted | 0-> no accepted connetion
    char* errorString;

    char buffer[255];                    // buffer used in socket recv()

public:
    HARTProtocolPLSocket();
    ~HARTProtocolPLSocket();

    void init();

    // Physical SAPs are sockets
    //void RESETrequest();								// *Request Physical Reset
	//void RESETconfirm();								// *Returns if Physical Reset will be implemanted (not necesary)

	void ENABLEindicate(char &state);					// 1 -> connection accepted | 0-> no accepted connetion

	void DATArequest(vector<unsigned char>& frameData);	// Writes frameData completly to socket
    void DATAindicate(vector<unsigned char>& frameData);// Returns the frame received from socket

	//void CYCLICindicate(char &local_status, HARTAddress &address, HARTCommand &command);

	//void ERRORindicate(char& status, char*& data);	// Returns Socket errors
};

#endif
