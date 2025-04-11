#ifndef _SPI_DRIVE_H_
#define _SPI_DRIVE_H_

#include <avr/io.h>
#include <stdint.h>

void start_spi(void);

uint16_t constrain(uint16_t x, uint16_t a, uint16_t b);

void spi_init();
void write_spi(uint8_t dataout);
uint8_t read_spi (void);
uint8_t transfer_spi (uint8_t data);
void close_spi(void);

void init_spislave(void);
uint8_t spi_slaveReceive(void);
uint16_t spi_slaveReceiveword(void);
void spi_slavesend(uint8_t data);

#endif /* _SPI_DRIEV_H_ */