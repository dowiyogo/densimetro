profibus: main.c 

	avr-gcc -mmcu=atmega1280 -std=gnu99 -Os DpAppl.c DpCfg.c DpDiag.c DpPrm.c ./DPV0_DRV/dp_if.c ./DPV0_DRV/dp_isr.c spi_drive.c spi_slave.c main.c -o profibus.o 

	avr-objcopy -j .text -j .data -O ihex profibus.o profibus.hex

programar: 
	avrdude -p m1280 -c arduino -P com12 -b 57600 -F -U flash:w:profibus.hex
#DpAppl.o: DpAppl.c DpAppl.h
	
#	avr-gcc -mmcu=atmega1280 -std=gnu99 -Os DpAppl.c -o DpAppl.o
programar_old:
	avrdude -p m1280 -c arduino -P com3 -b 57600 -F -U flash:w:profibus.hex

clean:
#	rm profibus.o DpAppl.o DpCfg.o DpDiag.o DpPrm.o dp_if.o dp_isr.o spi.o profibus
	rm profibus.o profibus.hex
