#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>

	#define SPI_SS_PIN PB0
	#define SPI_SCK_PIN PB1
	#define SPI_MOSI_PIN PB2
	#define SPI_MISO_PIN PB3
	

	// SPI clock modes
	#define SPI_MODE_0 0x00 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
	#define SPI_MODE_1 0x01 /* Setup (Rising) Sample (Falling) CPOL=0, CPHA=1 */
	#define SPI_MODE_2 0x02 /* Sample (Falling) Setup (Rising) CPOL=1, CPHA=0 */
	#define SPI_MODE_3 0x03 /* Setup (Falling) Sample (Rising) CPOL=1, CPHA=1 */

	// data direction
	#define SPI_LSB 1 /* send least significant bit (bit 0) first */
	#define SPI_MSB 0 /* send most significant bit (bit 7) first */

	// whether to raise interrupt when data received (SPIF bit received)
	#define SPI_NO_INTERRUPT 0
	#define SPI_INTERRUPT 1

	// slave or master with clock diviser
	#define SPI_SLAVE 0xF0
	#define SPI_MSTR_CLK4 0x00 /* chip clock/4 */
	#define SPI_MSTR_CLK16 0x01 /* chip clock/16 */
	#define SPI_MSTR_CLK64 0x02 /* chip clock/64 */
	#define SPI_MSTR_CLK128 0x03 /* chip clock/128 */
	#define SPI_MSTR_CLK2 0x04 /* chip clock/2 */
	#define SPI_MSTR_CLK8 0x05 /* chip clock/8 */
	#define SPI_MSTR_CLK32 0x06 /* chip clock/32 */



	// setup spi
	void setup_spi(void);

	// disable spi
	void disable_spi(void);

	// send and receive a byte of data (master mode)
	uint8_t send_spi(uint8_t out);

	// receive the byte of data waiting on the SPI buffer and
	// set the next byte to transfer - for use in slave mode
	// when interrupts are enabled.
	uint8_t received_from_spi(uint8_t out);

	

#endif
