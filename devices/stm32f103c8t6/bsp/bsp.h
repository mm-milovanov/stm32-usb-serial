#ifndef __bsp_h__
#define __bsp_h__

#include <ESDL/gpio.h>

void Bsp_init();
void Bsp_deinit();

extern gpio_pin_hdl_t   __LED__;

#endif /* __bsp_h__ */