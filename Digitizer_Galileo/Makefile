
test1: test_digitizer_1.c ./src/keyb.c ./src/keyb.h test_digitizer_1.o ./src/keyb.o pedestal.c pedestal.o
	g++ -I./src test_digitizer_1.o ./src/keyb.o -o test1 -lCAENDigitizer -DLINUX
	g++ -I./src pedestal.o ./src/keyb.o -o pedest -lCAENDigitizer -DLINUX


./src/keyb.o: ./src/keyb.c ./src/keyb.h 
	g++ -c ./src/keyb.c -o ./src/keyb.o -DLINUX

test_digitizer_1.o: test_digitizer_1.c 
	g++ -c -I./src test_digitizer_1.c -o test_digitizer_1.o -DLINUX

pedestal.o: pedestal.c 
	g++ -c -I./src pedestal.c -o pedestal.o -DLINUX
