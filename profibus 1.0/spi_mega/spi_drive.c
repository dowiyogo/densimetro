#define F_CPU 16000000UL
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <avr/io.h>
#include <util/delay.h>

void init_spi(void){

	


	DDRB= 0x87;
		
 }

uint8_t transfer_spi(uint8_t data){
	uint8_t in;
	 
	/*transfer out*/
	uint8_t he, heant=0,final=0;
	for (int i=1; i<=8; i++){
	//write_bit_gpio(clk,1);
	he= data>>8-i;
	final=he-heant;
	heant =he<<1;
	if(final)PORTB |=(1<<PB2);
	else PORTB&=~(1<<PB2);
	PORTB|=(1<<PB1);
	_delay_ms(1);
	PORTB &=~(1<<PB1);
	
	
	//printf("hex %u \n",final);
	

}
	//printf("write terminado %x \n",data);
	

	/*transfer in*/
	final=0;
	for (int i=1; i<=8; i++){
	PORTB|=(1<<PB1);
	if(PINB&(1<<PB3)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	_delay_ms(1);
	PORTB&=~(1<<PB1);
}
	//printf("valor recivido %u \n",final);
	//printf("Transferencia terminada \n");

	return final;
}

void write_spi(uint8_t data){

	uint8_t he, heant=0,final=0;
	/*transfer out*/
	//write_bit_gpio(mosi,0);
	//uint8_t he, heant=0,final;
	//write_bit_gpio(cs,0);

	for (int i=1; i<=8; i++){
	
	he= data>>8-i;
	final=he-heant;
	heant =he<<1;
	if(final)PORTB|=(1<<PB2);
	else PORTB&=~(1<<PB2);
	PORTB|=(1<<PB1);
	_delay_ms(1);
	PORTB &=~(1<<PB1);
	

	}
	//printf("write terminado %x \n",data);
	
	}


 uint8_t read_spi(void){
	
	uint8_t he,in=0, heant=0,leer,final=0;
	
	for (int i=1; i<=8; i++){
	PORTB|=(1<<PB1);
	if(PINB&(1<<PB3)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	
	PORTB&=~(1<<PB1);
}

	//printf("Transferencia terminada \n");

	

	return final;
}



void close_spi(void){

	PORTB|=(1<<PB0);
	//printf("Spi terminado \n");

}

void start_spi(void){

	PORTB&=~(1<<PB0);
	//printf("Spi terminado \n");

}

	
	


