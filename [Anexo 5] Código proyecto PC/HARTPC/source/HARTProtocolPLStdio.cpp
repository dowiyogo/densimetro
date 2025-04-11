#include "HARTProtocolPLStdio.hpp"

/* Const Definitions */
const char HARTProtocolPLStdio::map_hex_to_char[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


/* Constructor and Destructor*/
HARTProtocolPLStdio::HARTProtocolPLStdio ()
{
	HARTProtocolPL::state = INITIAL_STATE;
	HARTProtocolPL::error = NO_ERROR;
}

HARTProtocolPLStdio::~HARTProtocolPLStdio ()
{
	fpin  = NULL;
	fpout = NULL;
}

/* RESET services */
void HARTProtocolPLStdio::RESETrequest () {}
bool HARTProtocolPLStdio::RESETconfirm ()
{
	return true;
}

/* ENABLE services */
void HARTProtocolPLStdio::ENABLErequest (bool state) {}
bool HARTProtocolPLStdio::ENABLEindicate (bool& state)
{
	state = HARTProtocolPL::state;
	return !feof(stdin);
}
bool HARTProtocolPLStdio::ENABLEconfirm (bool& state)
{
	state = HARTProtocolPL::state;
	return true;
}

/* DATA services */
void HARTProtocolPLStdio::DATArequest (char* frameData, unsigned int fDsize)
{
	if (fpout != NULL) {
		for (unsigned int i=0; i<fDsize; i++) {
			putc('0', fpout);putc('x', fpout);
			putc(map_hex_to_char[(frameData[i]>>4)&0x0F], fpout);
			putc(map_hex_to_char[ frameData[i]    &0x0F], fpout);
			putc(HEXBYTE_DELIM, fpout);
		}
		putc('\n', fpout);
	} else {
		for (unsigned int i=0; i<fDsize; i++) {
			putchar('0');putchar('x');
			putchar(map_hex_to_char[(frameData[i]>>4)&0x0F]);
			putchar(map_hex_to_char[ frameData[i]    &0x0F]);
			putchar(HEXBYTE_DELIM);
		}
		putchar('\n');
	}
}
void HARTProtocolPLStdio::DATAindicate (char* frameData, unsigned int& fDsize)
{
	fDsize = 0;
	char c1, c2;
	if (fpin != NULL) {
		while ((c1 = fgetc(fpin)) != EOF && (c2 = fgetc(fpin)) != EOF
		//&& ((c1>47 && c1<58) || (c1>63 && c1<70))
		//&& ((c2>47 && c2<58) || (c2>63 && c2<70)) 
		) {
			if (c1=='\n' || c2=='\n')
				return;
			if (c1==COMMENT_CHAR || c2==COMMENT_CHAR) {
				while ((c1 = fgetc(fpin)) != EOF && (c2 = fgetc(fpin)) != EOF) {
					if (c1=='\n' || c2=='\n')
						return;
				}
				break;
			}		

			c1 = c1 - 48;
			c1 = (c1 > 16)? c1 - 7: c1;
			c2 = c2 - 48;
			c2 = (c2 > 16)? c2 - 7: c2;

			frameData[fDsize++] = (c1 << 4) | c2;
		}
	} else {
		while ((c1 = getc(stdin)) != EOF && (c2 = getc(stdin)) != EOF
		//&& ((c1>47 && c1<58) || (c1>63 && c1<70))
		//&& ((c2>47 && c2<58) || (c2>63 && c2<70)) 
		) {
			if (c1=='\n' || c2=='\n')
				return;
			if (c1==COMMENT_CHAR || c2==COMMENT_CHAR) {
				while ((c1 = getc(stdin)) != EOF && (c2 = getc(stdin)) != EOF) {
					if (c1=='\n' || c2=='\n')
						return;
				}
				break;
			}		

			c1 = c1 - 48;
			c1 = (c1 > 16)? c1 - 7: c1;
			c2 = c2 - 48;
			c2 = (c2 > 16)? c2 - 7: c2;

			frameData[fDsize++] = (c1 << 4) | c2;
		}
	}
	exit(0);
}
bool HARTProtocolPLStdio::DATAconfirm ()
{
	return true;		// Think positive
}

/* ERROR service */
HART_Protocol_Errors_types& HARTProtocolPLStdio::ERRORindicate (bool& status)
{
	return error;
}
