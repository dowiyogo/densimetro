#ifndef _SPI_SLAVE_H
#define _SPI_SLAVE_H

void init_spis(void);

uint8_t transfer_spis(uint8_t data);
void write_spis(uint8_t data);
uint8_t read_spis(void);
void close_spis(void);
void start_spis(void);



void SPI_SlaveInits(void);
uint8_t SPI_SlaveReceives(void);
void SPI_SlaveSends(uint8_t data);



#endif
