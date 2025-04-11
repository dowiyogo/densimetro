
#define F_CPU 16000000UL
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <util/atomic.h>


uint8_t tiempo=1;


//FOR MASTER 
 
void init_spi(void){

	


	DDRB|= (1<<PB0)|(1<<PB2);//PB0=SS;PB2=MOSI;PB1=MISO
	//DDRB=0x83;
	//PORTB|=(1<<PB3);
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

	_delay_us(tiempo);
	
	PORTL &=~(1<<PL1);


	_delay_us(tiempo);
	

}
	
	

	/*transfer in*/
	final=0;
	for (int i=1; i<=8; i++){
	PORTL|=(1<<PL1);
	if(PINB&(1<<PB1)){ 
	in=1;
		}else	in=0;

	final=(final<<1)+in;
	
	_delay_us(tiempo);
	
	PORTL&=~(1<<PL1);
	_delay_us(tiempo);	
	
}
	

	return final;
}

void write_spi(uint8_t data){

	uint8_t he, heant=0,count,final=0;
	/*transfer out*/
	

	for (int i=1; i<=8; i++){
	
	he= data>>(8-i);
	final=he-heant;
	heant =he<<1;
	if(final)PORTB|=(1<<PB2);
	else PORTB&=~(1<<PB2);
	PORTL|=(1<<PL1);
	
	_delay_us(tiempo);
	
	PORTL&=~(1<<PL1);
	
	_delay_us(tiempo);

	}

}
 uint8_t read_spi(void){
	
	uint8_t he,in=0, heant=0,leer,final=0;
	
	for (int i=1; i<=8; i++){
	PORTL|=(1<<PL1);
	_delay_us(tiempo);
	if(PINB&(1<<PB1)){ 
	in=1;
}else	in=0;

	final=(final<<1)+in;
	
	PORTL&=~(1<<PL1);
	_delay_us(tiempo);
	
}

	return final;
}



void close_spi(void){

	PORTB|=(1<<PB0);
	

}

void start_spi(void){

	PORTB&=~(1<<PB0);
	

}

	
	
//FOR SLave


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






