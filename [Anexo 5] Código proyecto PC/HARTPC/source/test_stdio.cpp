#include <stdio.h>
#include <vector>
#include "HARTProtocolDLL.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	HARTProtocolDLL dll(PL_CHANNEL_STDIO);
	HARTProtocolPLStdio out;

	out.RESETrequest();
	if(!out.RESETconfirm()) return 1;

	bool state = true;
	out.ENABLErequest(state);
	if(!out.ENABLEconfirm(state)) return 2;

	vector<char> data;
	while (out.DATAindicate(data)) {
		out.DATArequest(data);
	}

	return 0;
}