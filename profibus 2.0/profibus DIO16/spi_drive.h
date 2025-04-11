#ifndef _SPI_DRIVE_H
#define _SPI_DRIVE_H

void init_spi(void);


uint8_t transfer_spi(uint8_t data);
void write_spi(uint8_t data);
uint8_t read_spi(void);
void close_spi(void);
void start_spi(void);

void init_spislave(void);
uint8_t spi_slaveReceive(void);
void spi_slavesend(uint8_t data);

#endif
