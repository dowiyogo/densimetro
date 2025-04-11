#include "HARTProtocolPLSocket.hpp"

extern HART_Protocol_Errors HARTProtocolErrors;

HARTProtocolPLSocket::socketIP = (char*)"127.0.0.1";

HARTProtocolPLSocket::HARTProtocolPLSocket(){
    socketfp = 0;
    res      = NULL;
    state = 0;
}
HARTProtocolPLSocket::~HARTProtocolPLSocket(){
    close(socketfp);
}

void HARTProtocolPLSocket::init()
{
    struct sockaddr_in echoServAddr;
    if((socketfp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        //printf("%d\n", socketfp);
        HARTProtocolErrors.set(NOT_DEFINED_ERROR);
        state = 0;
        close(socketfp);
        return;
    }
    int ans = 1;
    if (setsockopt(socketfp, SOL_SOCKET, SO_REUSEADDR, &ans, sizeof(ans)) < 0) {
        //printf("bad socket config\n");
        HARTProtocolErrors.set(NOT_DEFINED_ERROR);
        state = 0;
        close(socketfp);
        return;
    }
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(socketPort);
    if(bind(socketfp, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
        //printf("puerto usado\n");
        HARTProtocolErrors.set(NOT_DEFINED_ERROR);//errorString = "bind";
        state = 0;
        close(socketfp);
        return;
    }
    if((listen(socketfp, 1)) < 0){
        //printf("no se puede escuchar\n");
        HARTProtocolErrors.set(NOT_DEFINED_ERROR);//errorString = "listening";
        state = 0;
        close(socketfp);
        return;
    }
    socklen_t slen;
    if((clintSockfp = accept(socketfp, (sockaddr*) NULL, &slen)) < 0){
        //  printf("error al aceptar conexion\n");
        HARTProtocolErrors.set(NOT_DEFINED_ERROR);//errorString = "accept";
        state = 0;
        close(socketfp);
        return;
    }
    state = 1;
}

void HARTProtocolPLSocket::ENABLEindicate(char& state)
{
    state = this->state;
}

void HARTProtocolPLSocket::DATArequest(vector<unsigned char>& frameData)
{
    int fDsize = frameData.size();
    for(int i=0;i<fDsize;i++){
        buffer[i] = frameData[i];
    }
    if((send(clintSockfp, buffer, fDsize, 0)) == -1){
        state = 0;
        HARTProtocolErrors.set(NOT_DEFINED_ERROR);//errorString = "socket closed";
        return;
    }
}
void HARTProtocolPLSocket::DATAindicate(vector<unsigned char>& frameData)
{
    frameData.clear();

    int readed = 0;
    while(readed < 1){
        if((readed = recv(clintSockfp, buffer, SOCKET_BUFFER_SIZE, MSG_PEEK)) == -1){
            state = 0;
            HARTProtocolErrors.set(NOT_DEFINED_ERROR);//errorString = "socket closed";
            return;
        }
    }
    for(int i=0;i<readed;i++){
        frameData.push_back(buffer[i]);
    }
    state = 1;
}
