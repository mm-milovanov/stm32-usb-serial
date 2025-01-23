#ifndef __spi5_h__
#define __spi5_h__

#include <lib-stm32drv/spi.h>

#include <stdbool.h>

void SPI5_init();

void SPI5_deinit();

void SPI5_write(char* request, int size);

void SPI5_read(char* request, char* response, int size);

#endif