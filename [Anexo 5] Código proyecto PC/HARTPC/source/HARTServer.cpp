#include <stdio.h>
#include <unistd.h>

#include <vector>
using namespace std;


//#include "HARTProtocolDeclarations.hpp"
#include "HARTProtocolDLL.hpp"
#include "HARTProtocolPLSocket.hpp"

extern HART_Protocol_Errors HARTProtocolErrors;

int main(int argc, char** argv){
    HARTProtocolErrors.push(NO_ERROR);

    HARTProtocolDLL hart;
    HARTAddress hartAddr((unsigned char)0x00);

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

    while(1){
        HARTCommand command;
        vector<unsigned char> data;

        while(1){
            hart.TRANSMITindicate(command, data);

            switch(command){
            default:{
                printf("A llegado carta:\t%d\n", static_cast<int>(command));
                break;
            }
            }
            hart.TRANSMITresponse(command, data);

            sleep(1);
        }
    }

    return 0;
}
