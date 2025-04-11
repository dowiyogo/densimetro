/*
* TODO asegurar kill de los procesos hijos aún ante error <- Se esta haciendo un return prematuro, sin matar el proceso hijo donde esta el servicior HART
*/

#include <vector>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <tester.h>

#include "HARTProtocolDLL.hpp"
#include "HARTProtocolPLSocket.hpp"

extern HART_Protocol_Errors HARTProtocolErrors;

unsigned char referenceArray[12] = {0xFF,0x00,0x00,0x81,0x02,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
// No Expansion byte & nor data
//                                 |CHKB|SIZE| CMD|ADDR|-DB-|-START-B-|-------- Preambule -----|

int connection();
int ping_pong();
int byteArray_ping_pong();
int loop_byteArray_ping_pong();
int connectThroughHPDLL();


int main(int argc, char** argv){
    TESTER_NAME("HARTProtocol test");

    TESTER_TEST("PLSock ::Local conection", connection);                        // Test 1
    TESTER_TEST("PLSock ::1 byte Indicate&Response", ping_pong);                        // Test 2
    TESTER_TEST("PLSock ::10 byte Indicate&Response", byteArray_ping_pong);             // Test 3
    TESTER_TEST("PLSock ::10 loop 10 byte Indicate&Response", loop_byteArray_ping_pong);// Test 4
    TESTER_TEST("DLL :: Device Connection", connectThroughHPDLL);                // Test 5
    TESTER_TEST("DLL :: Transmit Indicate&Response", HPDLLTransmit_ping_pong);   // Test 6
    TESTER_TEST("DLL :: Transmit (delm) wrong modulation -> Not FSK", HPDLLTransmit_delim_wrong_modulation); // Test 7
    TESTER_TEST("DLL :: Transmit (delm) wrong address type", HPDLLTransmit_delim_wrong_addr_type);
    TESTER_TEST("DLL :: Transmit (delm) not 0 expansion bytes", HPDLLTransmit_delim_expans_bytes_not_zero);
    TESTER_TEST("DLL :: Transmit (delm) frame not from master", HPDLLTransmit_delim_not_from_xmaster);
    //TESTER_TEST("DLL :: Transmit as bad address", HPDLLTransmit_bad_address);

    return 0;
}

/**************************************************************** HELPERS *****/
int startConnection(int& sockfd)
{
    struct sockaddr_in servAddr;
    if((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        close(sockfd);
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(socketIP);
    servAddr.sin_port = htons(socketPort);

    if (connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        return 2;       // couldn't connect

    return 0;
}
int rcvAll(int sockfd, unsigned char* rcvBuffer, int N)
{
    int readed = 0;
    while(readed < N){
        readed=recv(sockfd, rcvBuffer, N, MSG_PEEK);
        if(readed<0){
            printf("not all readed\n");
            return readed;
        }
    }
    return 0;
}
int sndAll(int sockfd, unsigned char* sndBuffer, int N)
{
    int writed = 0;
    while(writed < N){
        int sended = send(sockfd, &sndBuffer[writed], N-writed, 0);
        if(sended < 0)
            return 1;
        writed += sended;
    }
    return 0;
}

int hartDLLConnection(HARTProtocolDLL& hart)
{
    HARTAddress hartAddr((unsigned char)0x01);
    hart.configureDevice(hartAddr);

    hart.init();
    switch(HARTProtocolErrors.pull()){
        case(NO_ERROR):{
            break;
        }
        default:{
            printf("Error al iniciar conexión TCP/IP.\n");
            return 1;
            break;
        }
    }
    return 0;
}

/****************************************************************** TESTS *****/

/****** Physical Layer TESTS **************************************************/
int connection()
{
    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolPLSocket hartPL;
        hartPL.init();
        hartPL.~HARTProtocolPLSocket();
        exit(0);
    }else{
        usleep(1000);
        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        close(sockfd);
        kill(forkedID, SIGKILL);
    }
    return 0;
}

int ping_pong()
{
    vector<unsigned char> data;

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolPLSocket hartPL;
        hartPL.init();
        hartPL.DATAindicate(data);
        hartPL.DATArequest(data);
        hartPL.~HARTProtocolPLSocket();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, referenceArray, 1)!=0)
            return 4;
        unsigned char rcvBuffer[1] = {0};
        if(rcvAll(sockfd, rcvBuffer, 1) !=0)
            return 5;
        if(rcvBuffer[0] != referenceArray[0]){
            return 6;
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }
    return 0;
}

int byteArray_ping_pong()
{
    vector<unsigned char> data;

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolPLSocket hartPL;
        hartPL.init();
        hartPL.DATAindicate(data);
        hartPL.DATArequest(data);
        hartPL.~HARTProtocolPLSocket();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, referenceArray, 10)!=0)
            return 4;

        unsigned char rcvBuffer[10];
        if(rcvAll(sockfd, rcvBuffer, 10) !=0)
            return 5;
        for(int i=0;i<10;i++){
            if(rcvBuffer[i] != referenceArray[i]){
                return 6;
            }
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }
    return 0;
}

int loop_byteArray_ping_pong()
{
    vector<unsigned char> data;

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolPLSocket hartPL;
        hartPL.init();
        hartPL.DATAindicate(data);
        hartPL.DATArequest(data);
        hartPL.~HARTProtocolPLSocket();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        for(int i=0;i<10;i++){
            if(sndAll(sockfd, referenceArray, 10)!=0)
                return 4;

            unsigned char rcvBuffer[10];
            if(rcvAll(sockfd, rcvBuffer, 10) !=0)
                return 5;
            for(int i=0;i<10;i++){
                if(rcvBuffer[i] != referenceArray[i]){
                    return 6;
                }
            }
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }
    return 0;
}

/****** DLL TESTS *************************************************************/
int connectThroughHPDLL()
{
    vector<unsigned char> data;

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolErrors.push(NO_ERROR);

        HARTProtocolDLL hart;
        hartDLLConnection(hart);
        hart.~HARTProtocolDLL();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        close(sockfd);
        kill(forkedID, SIGKILL);
    }

    return 0;
}

int HPDLLTransmit_ping_pong()
{
    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolDLL hart;
        if(hartDLLConnection(hart)==0){
            HARTCommand command;
            vector<unsigned char> data;

            hart.TRANSMITindicate(command, data);
            switch(HARTProtocolErrors.pull()){
            case(FRAME_NOT_FROM_XMASTER):{
                printf("not master\n");
                data.clear();
                data.push_back(0x01);
                hart.TRANSMITresponse(command, data);
                break;
            }
            case(COMMAND_NOT_RECEIVED):{
                data.clear();
                printf("command : %#02x\n", (unsigned char)command);
                command = COMMAND0;
                hart.TRANSMITresponse(command, data);
                break;
            }
            default:{
                data.clear();
                hart.TRANSMITresponse(command, data);
                break;
            }
            }
        }
        hart.~HARTProtocolDLL();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, referenceArray, 12) !=0)
            return 4;

        unsigned char rcvBuffer[12];
        if(rcvAll(sockfd, rcvBuffer, 12) !=0)
            return 5;

        for(int i=0;i<12;i++){
            if(i<5 && rcvBuffer[i]!=(unsigned char)0xFF) return 6;              // Bad preamble value
            else if((i>4 && i<7) && rcvBuffer[i]!=(unsigned char)0xFF) return 7;// Bad start msg value
            else if(i==7 && rcvBuffer[i]!=(unsigned char)0x06) return 8;        // Bad delimeter value
            else if(i==8 && rcvBuffer[i]!=(unsigned char)0x81) return 9;        // Wrong device address or not to master
            else if(i==9 && rcvBuffer[i]!=(unsigned char)0x00) return 10;       // Wrong command
            else if(i==10 && rcvBuffer[i]!=(unsigned char)0x00) return 11;      // Bad data size count -> bad Address received at HART Server
            else if(i==11 && rcvBuffer[i-1]==(unsigned char)0x01 && rcvBuffer[i]!=(unsigned char)0x01) return 12; // Frame send not from master
            else if(i==11 && rcvBuffer[i]!=(unsigned char)0x78) return 13;      // Wrong checksum
        }//printf("%#02x\n",rcvBuffer[i]);

        close(sockfd);
        kill(forkedID, SIGKILL);
    }

    return 0;
}

int HPDLLTransmit_delim_wrong_modulation(){
    unsigned char lreferenceArray[12] = {0xFF,0x00,0x00,0x81,0x1A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // No Expansion byte nor data
    //                                 |CHKB|SIZE| CMD|ADDR|-DB-|-START-B-|-------- Preambule -----|
    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolDLL hart;
        if(hartDLLConnection(hart)==0){
            HARTCommand command;
            vector<unsigned char> data;

            hart.TRANSMITindicate(command, data);
            switch(HARTProtocolErrors.pull()){
            case(FRAME_NOT_FSK):{
                data.clear();
                data.push_back(0x01);
                hart.TRANSMITresponse(command, data);
                break;
            }
            default:{
                data.clear();
                hart.TRANSMITresponse(command, data);
                break;
            }
            }
        }
        hart.~HARTProtocolDLL();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, lreferenceArray, 12) !=0)
            return 4;

        unsigned char rcvBuffer[13];
        if(rcvAll(sockfd, rcvBuffer, 13) !=0)
            return 5;

        for(int i=0;i<13;i++){
            if(i<5 && rcvBuffer[i]!=(unsigned char)0xFF) return 6;              // Bad preamble value
            else if((i>4 && i<7) && rcvBuffer[i]!=(unsigned char)0xFF) return 7;// Bad start msg value
            else if(i==7 && rcvBuffer[i]!=(unsigned char)0x06) return 8;        // Bad delimeter value
            else if(i==8 && rcvBuffer[i]!=(unsigned char)0x81) return 9;        // Wrong device address or not to master
            // command cannot be tested, since ther is no command to read in HART Server
            else if(i==10 && rcvBuffer[i]!=(unsigned char)0x01) return 11;      // Bad data size count -> bad Address received at HART Server
            else if(i==11 && rcvBuffer[i]!=(unsigned char)0x01) return 12;      // Frame received with error
            else if(i==12 && rcvBuffer[i]!=(unsigned char)0x78) return 13;      // Wrong checksum
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }

    return 0;
}
int HPDLLTransmit_delim_wrong_addr_type(){
    unsigned char lreferenceArray[12] = {0xFF,0x00,0x00,0x81,0x82,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // No Expansion byte nor data
    //                                  |CHKB|SIZE| CMD|ADDR|-DB-|-START-B-|-------- Preambule -----|

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolDLL hart;
        if(hartDLLConnection(hart)==0){
            HARTCommand command;
            vector<unsigned char> data;

            hart.TRANSMITindicate(command, data);
            switch(HARTProtocolErrors.pull()){
            case(FRAME_NOT_ADDR_TYPE):{
                data.clear();
                data.push_back(0x01);
                hart.TRANSMITresponse(command, data);
                break;
            }
            default:{
                data.clear();
                hart.TRANSMITresponse(command, data);
                break;
            }
            }
        }
        hart.~HARTProtocolDLL();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, lreferenceArray, 12) !=0)
            return 4;

        unsigned char rcvBuffer[13];
        if(rcvAll(sockfd, rcvBuffer, 13) !=0)
            return 5;

        for(int i=0;i<13;i++){
            if(i<5 && rcvBuffer[i]!=(unsigned char)0xFF) return 6;              // Bad preamble value
            else if((i>4 && i<7) && rcvBuffer[i]!=(unsigned char)0xFF) return 7;// Bad start msg value
            else if(i==7 && rcvBuffer[i]!=(unsigned char)0x06) return 8;        // Bad delimeter value
            else if(i==8 && rcvBuffer[i]!=(unsigned char)0x81) return 9;        // Wrong device address or not to master
            // command cannot be tested, since ther is no command to read in HART Server
            else if(i==10 && rcvBuffer[i]!=(unsigned char)0x01) return 11;      // Bad data size count -> bad Address received at HART Server
            else if(i==11 && rcvBuffer[i]!=(unsigned char)0x01) return 12;      // Frame received with error
            else if(i==12 && rcvBuffer[i]!=(unsigned char)0x78) return 13;      // Wrong checksum
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }

    return 0;
}
int HPDLLTransmit_delim_expans_bytes_not_zero(){
    unsigned char lreferenceArray[12] = {0xFF,0x00,0x00,0x81,0x62,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // No Expansion byte nor data
    //                                  |CHKB|SIZE| CMD|ADDR|-DB-|-START-B-|-------- Preambule -----|

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolDLL hart;
        if(hartDLLConnection(hart)==0){
            HARTCommand command;
            vector<unsigned char> data;

            hart.TRANSMITindicate(command, data);
            switch(HARTProtocolErrors.pull()){
            case(FRAME_NOT_ZERO_EXPANS):{
                data.clear();
                data.push_back(0x01);
                hart.TRANSMITresponse(command, data);
                break;
            }
            default:{
                data.clear();
                hart.TRANSMITresponse(command, data);
                break;
            }
            }
        }
        hart.~HARTProtocolDLL();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, lreferenceArray, 12) !=0)
            return 4;

        unsigned char rcvBuffer[16];
        if(rcvAll(sockfd, rcvBuffer, 16) !=0)
            return 5;

        for(int i=0;i<16;i++){
            if(i<5 && rcvBuffer[i]!=(unsigned char)0xFF) return 6;              // Bad preamble value
            else if((i>4 && i<7) && rcvBuffer[i]!=(unsigned char)0xFF) return 7;// Bad start msg value
            else if(i==7 && rcvBuffer[i]!=(unsigned char)0x66) return 8;        // Bad delimeter value
            else if(i==8 && rcvBuffer[i]!=(unsigned char)0x81) return 9;        // Wrong device address or not to master
            // command cannot be tested, since ther is no command to read in HART Server
            else if(i==13 && rcvBuffer[i]!=(unsigned char)0x01) return 11;      // Bad data size count -> bad Address received at HART Server
            else if(i==14 && rcvBuffer[i]!=(unsigned char)0x01) return 12;      // Frame received with error
            else if(i==15 && rcvBuffer[i]!=(unsigned char)0xE7) return 13;      // Wrong checksum
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }

    return 0;
}
int HPDLLTransmit_delim_not_from_xmaster(){
    unsigned char lreferenceArray[12] = {0xFF,0x00,0x00,0x81,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // No Expansion byte nor data
    //                                  |CHKB|SIZE| CMD|ADDR|-DB-|-START-B-|-------- Preambule -----|

    int forkedID = fork();
    if(forkedID<0){
        return 1;
    }
    else if(forkedID==0){
        HARTProtocolDLL hart;
        if(hartDLLConnection(hart)==0){
            HARTCommand command;
            vector<unsigned char> data;

            hart.TRANSMITindicate(command, data);
            switch(HARTProtocolErrors.pull()){
            case(FRAME_NOT_FROM_XMASTER):{
                data.clear();
                data.push_back(0x01);
                hart.TRANSMITresponse(command, data);
                break;
            }
            default:{
                data.clear();
                hart.TRANSMITresponse(command, data);
                break;
            }
            }
        }
        hart.~HARTProtocolDLL();
        exit(0);
    }else{
        usleep(1000);

        int sockfd;
        int cresult = startConnection(sockfd);
        if(cresult != 0)
            return cresult;

        if(sndAll(sockfd, lreferenceArray, 12) !=0)
            return 4;

        unsigned char rcvBuffer[13];
        if(rcvAll(sockfd, rcvBuffer, 13) !=0)
            return 5;

        for(int i=0;i<13;i++){
            if(i<5 && rcvBuffer[i]!=(unsigned char)0xFF) return 6;              // Bad preamble value
            else if((i>4 && i<7) && rcvBuffer[i]!=(unsigned char)0xFF) return 7;// Bad start msg value
            else if(i==7 && rcvBuffer[i]!=(unsigned char)0x06) return 8;        // Bad delimeter value
            else if(i==8 && rcvBuffer[i]!=(unsigned char)0x81) return 9;        // Wrong device address or not to master
            // command cannot be tested, since ther is no command to read in HART Server
            else if(i==10 && rcvBuffer[i]!=(unsigned char)0x01) return 11;      // Bad data size count -> bad Address received at HART Server
            else if(i==11 && rcvBuffer[i]!=(unsigned char)0x01) return 12;      // Frame received with error
            else if(i==12 && rcvBuffer[i]!=(unsigned char)0x78) return 13;      // Wrong checksum
        }

        close(sockfd);
        kill(forkedID, SIGKILL);
    }

    return 0;
}
