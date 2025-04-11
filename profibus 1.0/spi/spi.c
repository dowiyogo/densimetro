#define F_CPU 16000000UL
#include <util/delay.h>
#include "spi.h"



	void setup_spi(void)
	{
		// specify pin directions for SPI pins on port B
			//DDRB |= (1<<SPI_MOSI_PIN); // output
			//DDRB &= ~(1<<SPI_MISO_PIN); // input
			//DDRB |= (1<<SPI_SCK_PIN);// output
			//DDRB=0x00;//DDRB |= (1<<SPI_SS_PIN);// output
			DDRB= (1<<PB0)|(1<<PB1)|(1<<PB2);
			SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);
			//SPCR=0x52;

	}

	void disable_spi()
	{	
		PORTB|=(1<<PB0);
		SPCR = 0;
	}

	uint8_t send_spi(uint8_t out)
	{
		PORTH|=(1<<PH4);
		//PORTH|=(1<<PH4);		
		//PORTB&=~(1<<PB0);		
		SPDR = out;
		//PORTB|=(1<<PB7);
		while (!(SPSR & (1<<SPIF)));
		_delay_ms(1000);
		PORTH&=~(1<<PH4);
		return SPDR;
	}

	uint8_t received_from_spi(uint8_t data)
	{

		data=SPDR;
		return data;
	}



