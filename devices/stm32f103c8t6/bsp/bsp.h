#ifndef __bsp_h__
#define __bsp_h__

#include <lib-stm32drv/gpio.h>
#include <lib-stm32drv/uart.h>

void __bsp_init__();
void __bsp_fini__();

extern uart_hdl_t       __USART1__;
extern gpio_pin_hdl_t   __LED_RED__;
extern gpio_pin_hdl_t   __LED_GREEN__;

#endif /* __bsp_h__ */