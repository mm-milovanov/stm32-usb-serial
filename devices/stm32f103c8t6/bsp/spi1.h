#ifndef __spi5_h__
#define __spi5_h__

#include <ESDL/spi.h>

#include <stdbool.h>

void SPI1_init();

void SPI1_deinit();

void SPI1_write(char* request, int size);

void SPI1_read(char* request, char* response, int size);

#endif