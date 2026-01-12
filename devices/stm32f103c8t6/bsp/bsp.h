#ifndef __bsp_h__
#define __bsp_h__

#include <ESDL/gpio.h>
#include <ESDL/uart.h>

void Bsp_init();
void Bsp_deinit();

extern uart_hdl_t   __USART1__;
extern gpio_pin_hdl_t   __LED__;

#endif /* __bsp_h__ */