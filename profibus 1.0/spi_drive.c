
#define F_CPU 16000000UL
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
//#include <unistd.h>
#include <avr/io.h>
#include <util/delay.h>
//#include <util/delay_basic.h>

void init_spi(void){

	


	DDRB|= (1<<PB0)|(1<<PB2);//PB0=SS;PB2=MOSI;PB3=MISO
	//DDRB=0x83;
	DDRL|=(1<<PL1);//PL0=CLK
		
 }

uint8_t transfer_spi(uint8_t data){
	uint8_t in,count;
	 
	/*transfer out*/
	uint8_t he, heant=0,final=0;
	for (int i=1; i<=8; i++){
	
	he= data>>(8-i);
	final=he-heant;
	heant =he<<1;
	if(final)PORTB |=(1<<PB2);
	else PORTB&=~(1<<PB2);
	PORTL|=(1<<PL1);
	
	_delay_us(1);
	
	PORTL &=~(1<<PL1);
	
	_delay_us(1);
	
	

}
	
	

	/*transfer in*/
	final=0;
	for (int i=1; i<=8; i++){
	PORTL|=(1<<PL1);
	if(PINB&(1<<PB3)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	_delay_us(1);
	PORTL&=~(1<<PL1);
	_delay_us(1);	
	
}
	

	return final;
}

void write_spi(uint8_t data){

	uint8_t he, heant=0,count,final=0;
	/*transfer out*/
	//write_bit_gpio(mosi,0);
	//uint8_t he, heant=0,final;
	//write_bit_gpio(cs,0);

	for (int i=1; i<=8; i++){
	
	he= data>>(8-i);
	final=he-heant;
	heant =he<<1;
	if(final)PORTB|=(1<<PB2);
	else PORTB&=~(1<<PB2);
	PORTL|=(1<<PL1);
	//_delay_loop_1(5);
	_delay_us(1);
	//_delay_ms(1); 
	/*for(uint8_t de=0;de<12;de++){	
	for(count=0;count<255;count++)_NOP();}*/
	PORTL&=~(1<<PL1);
	//_delay_loop_1(5);
	_delay_us(1);
	//_delay_ms(1);
	/*for(uint8_t de=0;de<12;de++){
	for(count=0;count<255;count++)_NOP();
	}*/
	
	//printf("write terminado %x \n",data);
	
	}

}
 uint8_t read_spi(void){
	
	uint8_t he,in=0, heant=0,leer,final=0;
	
	for (int i=1; i<=8; i++){
	PORTL|=(1<<PL1);
	_delay_us(1);
	if(PINB&(1<<PB3)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	
	PORTL&=~(1<<PL1);
	_delay_us(1);
	
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

	
	


