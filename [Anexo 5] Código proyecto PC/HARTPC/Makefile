###############################################################################
## Compilation Parameters
##
## Directories
HDIR=./header
SDIR=./source
ODIR=./object
LDIR=./library
BDIR=./bin

## 3rd Parties
TESTERH=./tester

## Compiler intructions
CC=g++
CFLAGS=-Wall -c -g -I$(HDIR) -I$(TESTERH) -std=c++98
LDFLAGS=-static -L$(LDIR)
AR=ar
AFLAGS=rcs


## Library Parameters
LIBHSOURCES_SOCKET=HARTProtocolDLL.cpp HARTProtocolPLSocket.cpp HARTProtocolErrors.cpp
LIBHOBJS_SOCKET=$(patsubst %.cpp, $(ODIR)/%.o, $(LIBHSOURCES_SOCKET))

LIBHSOURCES_STDIO=HARTProtocolDLL.cpp HARTProtocolPLStdio.cpp HARTProtocolErrors.cpp
LIBHOBJS_STDIO=$(patsubst %.cpp, $(ODIR)/%.o, $(LIBHSOURCES_STDIO))

## Test Parameters
TESTSRC_SOCKET=test.cpp
TESTOBJS_SOCKET=$(patsubst %.cpp, $(ODIR)/%.o, $(TESTSRC_SOCKET))

TESTSRC_STDIO=test_stdio.cpp
TESTOBJS_STDIO=$(patsubst %.cpp, $(ODIR)/%.o, $(TESTSRC_STDIO))

## DLL test with Stdio as PL
TESTSRC_DLL=test_dll.cpp
TESTOBJS_DLL=$(patsubst %.cpp, $(ODIR)/%.o, $(TESTSRC_DLL))

## Logger
LOGGERSOURCES=slave_logger.cpp slave_logger_test.cpp
LOGGEROBJS=$(patsubst %.cpp, $(ODIR)/%.o, $(LOGGERSOURCES)) 

###############################################################################
# Compilation Instructions
#
all: cleanall hart_stdio ctestdll
run:
	@./bin/server
pc_test:
	@cat packages | ./bin/ctestdll
teststdio:
	@./bin/cteststdio
logger:
	clogger
	run_logger
run_logger:
	@echo "Running logger"
	@./bin/serial_logger

# Compile all .cpp
$(ODIR)/%.o: $(SDIR)/%.cpp
	@$(CC) $(CFLAGS) $^ -o $@

# Static library compilation
hart_stdio: $(LIBHOBJS_STDIO)
	@echo "Building HART's Protocol Stack static library"
	@$(AR) $(AFLAGS) $(LDIR)/lib$@.a $^

# TEST Stdio as PL
cteststdio: $(LIBHOBJS_STDIO) $(TESTOBJS_STDIO)
	@echo "Building HART Protocol Stack Test with Stdio"
	@$(CC) $(LDFLAGS) -o $(BDIR)/$@ $^ -libhart_stdio.a

# TEST DLL
ctestdll: $(TESTOBJS_DLL)
	@echo "Building HART Protocol Stack DLL Test with Stdio"
	@$(CC) $(LDFLAGS) $^ -l hart_stdio -o $(BDIR)/$@

# LOGGER
serial_logger: $(LOGGEROBJS)
	@echo "Compiling and linking serial logger"
	@$(CC) $^ -o $(BDIR)/$@

clean:
	@rm -f $(BDIR)/* $(ODIR)/*
	@echo "Binaries and Object Removed"

cleanall:
	@rm -f $(BDIR)/* $(ODIR)/* $(LDIR)/*
	@echo "All old buildings removed"
