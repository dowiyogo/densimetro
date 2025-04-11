/***************************  Filename: main.c  ******************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:       Demo for PROFICHIP VPC3+                                  */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       eMail: support@profichip.com                     */
/*                          Phone: ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                                                                           */
/*****************************************************************************/

/** @file
 * Copyright (C) by profichip GmbH   All Rights Reserved. Confidential
 *
 * @brief Main function of PROFIBUS demo application.
 *
 * @author Peter Fredehorst
 * @version $Rev$
 */

/*****************************************************************************/
/* contents:

  - function prototypes
  - data structures
  - internal functions

*/
/*****************************************************************************/
/* include hierarchy */

#define F_CPU 16000000UL
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
//#include <unistd.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/wdt.h>

//#include <stdint.h>
#include "platform.h"
#include "spi_drive.h"
#include "spi_slave.h"
#include "DpAppl.h"


 
const int reset=3,baninter=2,salida=1,entrada=0;
int entryreset=1;
int entrybaninter=1;
uint8_t wdt_counter=0;
uint8_t data, rdata;
uint8_t *bdata;
uint8_t ciclo=1,centena; 

//void wdt_first(void) __attribute__((naked)) __attribute__((section(".init3")));
/*---------------------------------------------------------------------------*/
/* Global variables                                                      */
/*---------------------------------------------------------------------------*/





/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void main( void );

/*void wdt_first(void) {

	MCUSR = 0; 
	wdt_disable();
}

void wdt_init() {
	cli();
	WDTCSR = (1<<WDCE) | (1<<WDE);  

	wdt_enable(WDTO_1S);
	sei();

}

void reset_wdt() {
	wdt_counter = 0;
	wdt_reset();
}
 
*/
/*---------------------------------------------------------------------------*/
/* VPC3 reset settings                                                       */
/*---------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* function: Swap16                                                         */
/*--------------------------------------------------------------------------*/
void Swap16( uint16_t MEM_PTR w )
{
   //Motorola <--> PC Format

   *w = ( *w>>8 | *w<<8 );    // 1 2  <-->  2 1
}//void Swap16( uint16_t MEM_PTR w )

/*--------------------------------------------------------------------------*/
/* function: SwapWord                                                       */
/*--------------------------------------------------------------------------*/
uint16_t SwapWord( uint16_t x )
{
   return ( x >> 8 ) | ( (uint8_t)x << 8 );
}//uint16_t SwapWord( uint16_t x )

/*--------------------------------------------------------------------------*/
/* function: SwapDWord                                                      */
/*--------------------------------------------------------------------------*/
uint32_t SwapDWord( uint32_t x )
{
MEM_UNSIGNED8_PTR ptr;
MEM_UNSIGNED8_PTR ptrx;
uint32_t             dwRetValue;

   ptr  = (MEM_UNSIGNED8_PTR)&dwRetValue;
   ptrx = (MEM_UNSIGNED8_PTR)&x;

   *ptr++ = *(ptrx+3);
   *ptr++ = *(ptrx+2);
   *ptr++ = *(ptrx+1);
   *ptr++ = *(ptrx+0);
   return dwRetValue;
}//uint32_t SwapDWord( uint32_t x )

/*--------------------------------------------------------------------------*/
/* function: Swap32                                                         */
/*--------------------------------------------------------------------------*/
void Swap32( uint32_t MEM_PTR x )
{
   uint8_t p;

   union
   {
      uint32_t a;
      uint8_t b[4];
   }z;

   //Motorola <--> PC Format
   z.a    = *x;             // 1 2 3 4

   p      = z.b[0];         // 1
   z.b[0] = z.b[3];         // 4 2 3 4
   z.b[3] = p;              // 4 2 3 1
   p      = z.b[1];         //   2
   z.b[1] = z.b[2];         // 4 3 3 1
   z.b[2] = p;              // 4 3 2 1

   *x = z.a;                // 4 3 2 1
}//void Swap32( uint32_t MEM_PTR x )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_SetResetVPC3Channel1                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Set VPC3+ reset.
 *
 * @attention The VPC3+ reset is high-active!
 *
 */

void init_PWM(void)
{
    TCCR0A|=(1<<WGM00)|(1<<WGM01)|(1<<COM0A1);
    TCCR0B|=(1<<CS01)|(1<<CS00);
    

    DDRB|=(1<<PB7);
}

void setPWM(uint8_t duty)
{
   OCR0A = duty;
}

void DpAppl_SetResetVPC3Channel1( void ){

if (entryreset){
	DDRH =(1<<PH4);
	entryreset=0;
}
			
  PORTH|=(1<<PH4);
  /*_delay_ms(1);
  PORTH &=~(1<<PH4);
*/
	//printf("reset ok!\n");
    
}//void DpAppl_SetResetVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_ClrResetVPC3Channel1                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Clear VPC3+ reset.
 *
 * @attention The VPC3+ reset is high-active!
 *
 */
void DpAppl_ClrResetVPC3Channel1( void){

  /*PORTH|=(1<<PH4);
  _delay_ms(1);*/
  PORTH &=~(1<<PH4);
	  
}//void DpAppl_ClrResetVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_EnableInterruptVPC3Channel1                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief Enable VPC3+ interrupt.
 */
void DpAppl_EnableInterruptVPC3Channel1( void )
{
	EICRA|=(1<<ISC31);

	EIMSK|=(1<<INT3);
	sei();
	
	//_delay_ms(1);
   /** @todo Add your own code here! */
}//void DpAppl_EnableInterruptVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_DisableInterruptVPC3Channel1                             */
/*---------------------------------------------------------------------------*/
/**
 * @brief Disable VPC3+ interrupt.
 *
 */
void DpAppl_DisableInterruptVPC3Channel1( void )
{
	cli();
	//EIMSK&=~(1<<INT3);
	//cli();
	//_delay_ms(1);
   /** @todo Add your own code here! */
}//void DpAppl_DisableInterruptVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_EnableInterruptVPC3Sync                                  */
/*---------------------------------------------------------------------------*/
/**
 * @brief Enable VPC3+ isochronous interrupt.
 *
 * @attention Is only supported from VPC3+S!
 *
 */
void DpAppl_EnableInterruptVPC3Sync( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_EnableInterruptVPC3Sync( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_DisableInterruptVPC3Sync                                 */
/*---------------------------------------------------------------------------*/
/**
 * @brief Disable VPC3+ isochronous interrupt.
 *
 * @attention Is only supported from VPC3+S!
 *
 */
void DpAppl_DisableInterruptVPC3Sync( void )
{
   
   /** @todo Add your own code here! */
}//void DpAppl_DisableInterruptVPC3Sync( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_EnableAllInterrupts                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Enable all microcontroller interrupts.
 *
 */
void DpAppl_EnableAllInterrupts( void )
{
	DpAppl_EnableInterruptVPC3Channel1( );
	
   /** @todo Add your own code here! */
}//void DpAppl_EnableAllInterrupts( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_DisableAllInterrupts                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Disable all microcontroller interrupts.
 *
 */
void DpAppl_DisableAllInterrupts( void )
{

	
DpAppl_DisableInterruptVPC3Channel1();

   /** @todo Add your own code here! */
}//void DpAppl_DisableAllInterrupts( void )

/*---------------------------------------------------------------------------*/
/* function: Vpc3Wait_1ms                                                    */
/*---------------------------------------------------------------------------*/
/**
 * @brief Wait some time.
 *
 */
void Vpc3Wait_1ms( void )
{
   	_delay_ms(1);
	

}//void Vpc3Wait_1ms( void )

/*---------------------------------------------------------------------------*/
/* function: Vpc3Write                                                       */
/*---------------------------------------------------------------------------*/
/**
 * @brief Write a byte to VPC3+.
 *
 * @attention This function is only necessary with VPC3+S in SPI- or IIC-Mode!
 *
 * @param[in]wAddress Address in VPC3+
 * @param[in]bData Data
 */
#if VPC3_SERIAL_MODE
void Vpc3Write( VPC3_ADR wAddress, uint8_t bData )
{	
	DpAppl_DisableInterruptVPC3Channel1();
	
	start_spi();
	
	write_spi(0X12);
		
	data=(uint8_t)(wAddress >> 8);
	write_spi(data);// direccion
	//while(!read_bit_gpio(1));
	
	data=(uint8_t)(wAddress);
	write_spi(data);// direccion
	//while(!read_bit_gpio(1));

	write_spi(bData);// datos
	//while(!read_bit_gpio(1));
	

	close_spi();
	
	DpAppl_EnableInterruptVPC3Channel1();


   /** @todo Add your own code here! */
}//void Vpc3Write( VPC3_ADR wAddress, uint8_t bData )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: Vpc3Read                                                        */
/*---------------------------------------------------------------------------*/
/**
 * @brief Read one byte from VPC3+.
 *
 * @attention This function is only necessary with VPC3+S in SPI- or IIC-Mode!
 *
 * @param[in]wAddress Address in VPC3+
 * @return value of wAddress
 */
#if VPC3_SERIAL_MODE
uint8_t Vpc3Read( VPC3_ADR wAddress )
{
	
    DpAppl_DisableInterruptVPC3Channel1();
	

   	start_spi();
	//printf("ok0\n");  
	write_spi(0x13);
	//while(!read_bit_gpio(1));
	//printf("ok1\n");
	data=(uint8_t)(wAddress >> 8);
	write_spi(data);// direccion
	//while(!read_bit_gpio(1));
	//printf("ok2\n");
	
	data=(uint8_t)(wAddress);
	//printf("ok3\n");
	
	rdata=transfer_spi(data);// direccion
	//while(!read_bit_gpio(2));
		
   	close_spi();
	
	 DpAppl_EnableInterruptVPC3Channel1();


   return rdata;
}//uint8_t Vpc3Read( VPC3_ADR wAddress )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: Vpc3MemSet                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Fill block of VPC3+ memory.
 *
 * @param[in]wAddress Address of the block of memory to fill.
 * @param[in]bValue  Value to be set.
 * @param[in]wLength Number of bytes to be set to the value.
 */
#if VPC3_SERIAL_MODE
void Vpc3MemSet( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength )
{
	uint16_t i;
	
			

      DpAppl_DisableInterruptVPC3Channel1();
	
	start_spi();
	write_spi(0x02);
	//while(!read_bit_gpio(2));
	
	data=(uint8_t)(wAddress >> 8);
	write_spi(data);// direccion
	//while(!read_bit_gpio(2));
	
	data=(uint8_t)(wAddress);
	write_spi(data);// direccion
	//while(!read_bit_gpio(2));
	
	for(i=0;i<wLength;i++) write_spi(bValue);
	
	
	close_spi();
  DpAppl_EnableInterruptVPC3Channel1();
   
}//void Vpc3MemSet( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: Vpc3MemCmp                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Compare two blocks of VPC3+ memory.
 *
 * @param[in]pToVpc3Memory1 Pointer to block of memory.
 * @param[in]pToVpc3Memory2 Pointer to block of memory.
 * @param[in]wLength Number of bytes to compare.
 * @return 0 Indicates that the contents of both memory blocks are equal.
 * @return 1 Indicates that the contents of both memory blocks are not equal.
 */
#if VPC3_SERIAL_MODE
uint8_t Vpc3MemCmp( VPC3_UNSIGNED8_PTR pToVpc3Memory1, VPC3_UNSIGNED8_PTR pToVpc3Memory2, uint16_t wLength )
{
 

uint8_t bRetValue;
uint16_t i;
		
   bRetValue = 0;
   for( i = 0; i < wLength; i++ )
   {
      if( Vpc3Read( (VPC3_ADR)pToVpc3Memory1++ ) != Vpc3Read( (VPC3_ADR)pToVpc3Memory2++ ) )
      {
         bRetValue = 1;
         break;
      }//if( Vpc3Read( (VPC3_ADR)pToVpc3Memory1++ ) != Vpc3Read( (VPC3_ADR)pToVpc3Memory2++ ) )
   }//for( i = 0; i < wLength; i++ )

   return bRetValue;
}//uint8_t Vpc3MemCmp( VPC3_UNSIGNED8_PTR pToVpc3Memory1, VPC3_UNSIGNED8_PTR pToVpc3Memory2, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: CopyToVpc3                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Copy block of memory to VPC3+.
 *
 * @param[in]pToVpc3Memory Pointer to the destination array where the content is to be copied.
 * @param[in]pLocalMemory Pointer to the source of data to be copied.
 * @param[in]wLength Number of bytes to copy.
 */
#if VPC3_SERIAL_MODE
void CopyToVpc3( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength )
{
	uint16_t wAddress;
	uint16_t i;
		
	

      DpAppl_DisableInterruptVPC3Channel1();
   
	start_spi();
	write_spi(0x02);
	//while(!read_bit_gpio(2));
	
	wAddress = (VPC3_ADR)pToVpc3Memory;
	
	data=(uint8_t)(wAddress>>8);
	
	write_spi(data);
	//while(!read_bit_gpio(2));
	
	data=(uint8_t)(wAddress);
	
	write_spi(data);
	//while(!read_bit_gpio(2));
	
	for (i=0;i<wLength;i++){
	write_spi(*pLocalMemory++);
	//while(!read_bit_gpio(2));

	}
	close_spi();
	DpAppl_EnableInterruptVPC3Channel1();



}//void CopyToVpc3( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: CopyFromVpc3                                                    */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
/**
 * @brief Copy block of memory from VPC3+.
 *
 * @param[in]pLocalMemory Pointer to the destination array where the content is to be copied.
 * @param[in]pToVpc3Memory Pointer to the source of data to be copied.
 * @param[in]wLength Number of bytes to copy.
 */
void CopyFromVpc3( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength )
{
   
   uint16_t wAddress;
   uint16_t i;
   uint8_t lectura;
 

      DpAppl_DisableInterruptVPC3Channel1();
   
   

   start_spi();
   write_spi(0x03);
   //while(!read_bit_gpio(2));
    
   wAddress = (VPC3_ADR)pToVpc3Memory;
   
   data=(uint8_t)(wAddress>>8);

   write_spi(data);
   //while(!read_bit_gpio(2));
   
   data=(uint8_t)(wAddress);

   write_spi(data);
  // while(!read_bit_gpio(2));
   
   for(i=0; i<wLength; i++){
   
	pLocalMemory[i]=read_spi();
	
       // while(!read_bit_gpio(2));
   }

   close_spi();
    DpAppl_EnableInterruptVPC3Channel1();
   
   

}//void CopyFromVpc3( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: TestVpc3_01                                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Hardware test of VPC3+.
 * If you get problems with reading VPC3+, you should read first the status register address 5.
 * The default value is CFhex (VPC3+C) or EFhex (VPC3+S).
 * Check reset signal of VPC3+ (notice: reset is high active).
 *
 */
void TestVpc3_01( void )
{
uint8_t bValue;

   DpAppl_SetResetVPC3Channel1();
   Vpc3Wait_1ms();
   DpAppl_ClrResetVPC3Channel1();

   
      #if VPC3_SERIAL_MODE
         bValue = Vpc3Read(0x05);
	if (bValue==0Xef){
	 PORTB |=(1<<PB7);
	_delay_ms(1000);
	PORTB &=~(1<<PB7);_delay_ms(200);}else{PORTB |=(1<<PB7);
	_delay_ms(100);
	PORTB &=~(1<<PB7);}
      #else
         bValue = *((unsigned char *)0x28005); //address depends on hardware!!!!!
      #endif//#if VPC3_SERIAL_MODE
   //while(1)
}//void TestVpc3_01( void )






void TestCopyFromVpc3( void )
{
uint8_t abValue[2];
uint8_t bValue;
DpAppl_SetResetVPC3Channel1();
Vpc3Wait_1ms();
DpAppl_ClrResetVPC3Channel1();
Vpc3Write(0x100, 0x75);
Vpc3Write(0x101, 0x47);
bValue = Vpc3Read(0x100); /* bValue = 0x75? */
//PORTB|=(1<<PB7);_delay_ms(10);PORTB&=~(1<<PB7);_delay_ms(10);
//if(bValue==0x75) {PORTB|=(1<<PB7);_delay_ms(10);PORTB&=~(1<<PB7);_delay_ms(10);}
//start_spis();
//write_spis(bValue);
bValue = Vpc3Read(0x101);
//write_spis(bValue);
//close_spis();
//if(bValue==0x47) {PORTB|=(1<<PB7);_delay_ms(500);PORTB&=~(1<<PB7);_delay_ms(10);} /* bValue = 0x47? */
CopyFromVpc3( &abValue[0], (VPC3_UNSIGNED8_PTR)0x100, 2 );
start_spis();
write_spis(abValue[0]);
write_spis(abValue[1]);
close_spis();
//if((abValue[0]==0x75)*(abValue[1]==0x47)) PORTB|=(1<<PB7);

}
/*---------------------------------------------------------------------------*/
/* function: main                                                            */
/*---------------------------------------------------------------------------*/
/**
 * @brief Main-function.
 *
 */
void main( void )
{
	//wdt_init();
	//wdt_enable(WDTO_4S);
	DpAppl_SetResetVPC3Channel1();
	_delay_ms(5);
	DpAppl_ClrResetVPC3Channel1();
	
	int g=1, pendiente=1;
	
	DDRB |=((1<<PB7)|(1<<PB6));
	DDRB|=(1<<PD3);
	DDRA=0x0f;DDRC=0x3f;
	PORTD|=(1<<PD3);
	
	init_spi();
	
	
	//TestVpc3_01();
	//init_spis();
	
	
	PORTB|=(1<<PB7);
	_delay_ms(3000);
	PORTB &=~(1<<PB7);
	_delay_ms(100);
	//TestCopyFromVpc3();
	//wdt_init();
	DpAppl_ProfibusInit();
	init_PWM();
	init_spislave();
	uint8_t display=22;
  while(1)
   {
      // call PROFIBUS
	/*if(g){
	wdt_init();/*
	  DpAppl_DisableInterruptVPC3Channel1();
	PORTB |=(1<<PB6);
	_delay_ms(3000);
	PORTB &=~(1<<PB6);
DpAppl_EnableInterruptVPC3Channel1();
	g=0;
	
	
}*/
	
    
	if (pendiente){
	ciclo +=2;
	//sDpAppl.abDpInputData[0]=ciclo;
	//setPWM(sDpAppl.abDpInputData[0]);
	
				
	
		if (ciclo>250)pendiente=0;
	}
	
	if(!pendiente){
	ciclo -=2;
	//sDpAppl.abDpInputData[0]=ciclo;
	//setPWM(sDpAppl.abDpInputData[0]);
	
	if (ciclo<5){ pendiente=1;}
	
}
	_delay_ms(1);
	/*PORTA=ciclo%10;
	PORTC=(ciclo%100)/10;
		centena=ciclo/100;
	if(centena==1) PORTC|=(1<<PC4);
			else if(centena==2) PORTC|=((1<<PC4)|(1<<PC5));else PORTC&=~((1<<PC4)|(1<<PC5));*/
	sDpAppl.abDpInputData[0]=ciclo;
	setPWM(sDpAppl.abDpInputData[0]);
	//sDpAppl.abDpInputData[1]=5;
      DpAppl_ProfibusMain();
	  
	  
	  //PORTB&=~((1<<PB5)|(1<<PB6));
	  //reset_wdt();
	  //wdt_reset();
	 
	
	/*PORTB &=~(1<<PB6);
	_delay_ms(500);*/

   }//while(1)*/
}



ISR(INT3_vect){


//VPC3_Poll();
//PORTB|=(1<<PB7);_delay_ms(100);PORTB&=~(1<<PB7);_delay_ms(10);
sDpAppl.abDpInputData[1]=spi_slaveReceive();

/*for (int i=0; i<rec;i++){

	PORTB|=(1<<PB6);
	_delay_ms(300);
	PORTB&=~(1<<PB6);
	_delay_ms(100);
	}
*/

 }
 
 
 
 
 
/*ISR(WDT_vect)
{
	wdt_counter++;
	if (wdt_counter < 60) { 
		wdt_reset();
	} else {
		WDTCSR = (1<<WDCE) | (1<<WDE);	// Enable the WD Change Bit - configure mode
		WDTCSR = (1<<WDE) | (1<<WDP0);	// set reset flag (WDE) and 16ms (WDP0)
	}
}*/



//void main( void ) 

/*---------------------------------------------------------------------------*/
/* interrupt: external interrupt 0                                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

