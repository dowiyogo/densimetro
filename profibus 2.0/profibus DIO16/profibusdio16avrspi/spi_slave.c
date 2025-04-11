
#define F_CPU 16000000UL
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
//#include <unistd.h>
#include <avr/io.h>
#include <util/delay.h>
//#include <util/delay_basic.h>

void init_spis(void){

	

	DDRD|= ((1<<PD0)|(1<<PD2)|(1<<PD3));//mosi,clk,ss
	//DDRB=0x83;
			
 }

uint8_t transfer_spis(uint8_t data){
	uint8_t in,count;
	 
	/*transfer out*/
	uint8_t he, heant=0,final=0;
	for (int i=1; i<=8; i++){
	he= data>>(8-i);
	final=he-heant;
	heant =he<<1;
	if(final)PORTD |=(1<<PD0);
	else PORTD&=~(1<<PD0);
	PORTD|=(1<<PD2);
	_delay_ms(10);
	//_delay_us(1);
	//_delay_loop_1(5);
	/*for(uint8_t de=0;de<12;de++){	
	for(count=0;count<255;count++)_NOP();}*/
	PORTD &=~(1<<PD2);
	/*for(uint8_t de=0;de<12;de++){	
	for(count=0;count<255;count++)_NOP();}*/

	_delay_ms(10);
	//_delay_us(1);
	//_delay_loop_1(5);
	//printf("hex %u \n",final);
	

}
	//printf("write terminado %x \n",data);
	

	/*transfer in*/
	final=0;
	for (int i=1; i<=8; i++){
	PORTD|=(1<<PD2);
	if(PIND&(1<<PD1)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	_delay_ms(10);
	//_delay_us(1);
	//_delay_loop_1(5);
	/*for(uint8_t de=0;de<12;de++){	
	for(count=0;count<255;count++)_NOP();}*/
	PORTD&=~(1<<PD2);
	//_delay_us(1);	
	_delay_ms(10);
	//_delay_loop_1(5);
	/*for(uint8_t de=0;de<12;de++){	
	for(count=0;count<255;count++)_NOP();}*/
}
	//printf("valor recivido %u \n",final);
	//printf("Transferencia terminada \n");

	return final;
}

void write_spis(uint8_t data){

	uint8_t he, heant=0,count,final=0;
	/*transfer out*/
	//write_bit_gpio(mosi,0);
	//uint8_t he, heant=0,final;
	//write_bit_gpio(cs,0);

	for (int i=1; i<=8; i++){
	
	he= data>>(8-i);
	final=he-heant;
	heant =he<<1;
	if(final)PORTD|=(1<<PD0);
	else PORTD&=~(1<<PD0);
	PORTD|=(1<<PD2);
	//_delay_loop_1(5);
	//_delay_us(1);
	_delay_ms(50); 
	/*for(uint8_t de=0;de<12;de++){	
	for(count=0;count<255;count++)_NOP();}*/
	PORTD&=~(1<<PD2);
	//_delay_loop_1(5);
	//_delay_us(1);
	_delay_ms(50);
	/*for(uint8_t de=0;de<12;de++){
	for(count=0;count<255;count++)_NOP();
	}*/
	
	//printf("write terminado %x \n",data);
	
	}

}
 uint8_t read_spis(void){
	
	uint8_t he,in=0, heant=0,leer,final=0;
	
	for (int i=1; i<=8; i++){
	PORTD|=(1<<PD2);
	if(PIND&(1<<PD1)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	
	PORTD&=~(1<<PD2);
}

	//printf("Transferencia terminada \n");

	

	return final;
}



void close_spis(void){

	PORTD|=(1<<PD3);
	//printf("Spi terminado \n");

}

void start_spis(void){

	PORTD&=~(1<<PD3);
	//printf("Spi terminado \n");

}

	
	


