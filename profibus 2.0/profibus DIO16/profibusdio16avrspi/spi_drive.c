#define F_CPU 16000000UL
#include "spi_drive.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

void spi_init()
// Initialize pins for spi communication
{
    DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
	DDRB&=~(1<<PB3);
    // Define the following pins as output
    //DDR_SPI |= ((1<<DD_MOSI)|(1<<DD_SS)|(1<<DD_SCK));

    
    SPCR = (1<<SPE)|               // SPI Enable
            (0<<SPIE)|              // SPI Interupt Disable
            (0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
            (1<<MSTR)|              // Master/Slave select (0<<SPR0)|  
			(1<<SPR0)|(0<<SPR1)|	// SPI Clock Rate
            (0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0<<CPHA);             // Clock Phase (0:leading / 1:trailing edge sampling)

    //SPSR = (1<<SPI2X);              // Double Clock Rate
    
}






void write_spi (uint8_t dataout)
// Shift full array through target device
{
        SPDR = dataout;
        while((SPSR & (1<<SPIF))==0);
		//PORTB|=(1<<PB1)|(1<<PB2);
             }

uint8_t read_spi  (void)
// Shift full array to target device without receiving any byte
{
            
                SPDR = 0x00;
             while((SPSR & (1<<SPIF))==0);
				return SPDR;
}

uint8_t transfer_spi (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
    SPDR = data;
    while((SPSR & (1<<SPIF))==0);
	//PORTB&=~(0<<PB0);
	
	//PORTB|=(1<<PB1)|(1<<PB2);
	SPDR = 0x00;
	while((SPSR & (1<<SPIF))==0);
	
    return SPDR;
}

uint16_t constrain(uint16_t x, uint16_t a, uint16_t b){

if (x<a) return a;
else if (b<x) return b;
else return x ;} 

void start_spi(void){

PORTB&=~(1<<PB0);

}

void close_spi(void){

PORTB|=(1<<PB0);

}




void init_spislave(void){	

		DDRH|=(1<<PH0);//MISO PIN out
		EICRA|=(1<<ISC31);//interrupcion cuando se registre una caida en el voltaje
		EIMSK|=(1<<INT3);// interrupcion para recibir los datos
		sei();//Habilita interrupcion
		
		}


uint8_t spi_slaveReceive(void)
{
	uint8_t he=1,in=0,i=0, heant=0,leer,final=0;
	
	while(he<=8){		
	
	if(PINJ&(1<<PJ0)){
			
			
		if(PINH&(1<<PH1)){
			in=1; 
			final=(final<<1)+in;
			
			he++;
			}else	{
			in=0;
			final=(final<<1)+in;
			
			he++;}
		
	while(PINJ&(1<<PJ0));
		
						}
				}
		
		
		//PORTB&=~(1<<PB7);
		
	return final;
}


uint16_t spi_slaveReceiveword(void)
{
	uint16_t he=1,in=0,i=0, heant=0,leer,final=0;
	
	while(he<=16){		
		
	if(PINJ&(1<<PJ0)){
			
			
		if(PINH&(1<<PH1)){
			in=1; 
			final=(final<<1)+in;
			
			he++;
			}else	{
			in=0;
			final=(final<<1)+in;
			
			he++;}
	
	while(PINJ&(1<<PJ0));	}			
		}
	
			
	
	return final;
}

void spi_slavesend(uint8_t data){


uint8_t he=1,in=0,i=0, heant=0,leer,final=0;
	
	while(he<=8){		
	
	if(PINJ&(1<<PJ0)){
	
	
	he= data>>(8-i);
	final=he-heant;
	heant =he<<1;
	if(final)PORTH|=(1<<PH1);
	else PORTH&=~(1<<PH1);
			
	while(PINJ&(1<<PJ0));
		
						}
				}
		
	}

