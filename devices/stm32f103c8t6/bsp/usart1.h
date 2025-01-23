#ifndef __bsp_usart1_h__
#define __bsp_usart1_h__

#include <stdint.h>

void USART1_init();

void USART1_deinit();

void USART1_transmit(uint8_t* buf, int size);

#endif // __bsp_usart1_h__