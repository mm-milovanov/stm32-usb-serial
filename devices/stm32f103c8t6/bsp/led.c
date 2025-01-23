#include "led.h"

#include <ESDL/gpio.h>
#include <cmsis/stm32f1xx.h>

gpio_pin_hdl_t __LED__;

void Led_init()
{
    const gpio_pin_conf_t pin_conf = {
        .gpioX = GPIOC,
        .idx = 13,
        .cnf = GPIO_CNF_PUSH_PULL_OUTPUT,
        .mode = GPIO_MODE_OUTPUT_50MHZ,
        .clk = GPIOC_CLK_EN
    };

    gpio_pin_hdl_ctor(&__LED__, &pin_conf);
}

void Led_deinit()
{
    gpio_pin_hdl_dtor(&__LED__);
}