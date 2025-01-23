#include <bsp/leds.h>

#include <defconfig.h>
#include <lib-stm32drv/gpio.h>
#include <cmsis/stm32f4xx.h>

gpio_pin_hdl_t __LED_RED__;
gpio_pin_hdl_t __LED_GREEN__;

void __leds_init__()
{
    const gpio_pin_conf_t pin_red_conf = {
        ._gpioX = GPIOG,
        ._idx = 14,
        ._moder = GPIO_MODER_OUTPUT,
        ._otyper = GPIO_OTYPER_OUTPUT_PP,
        ._ospeedr = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr = GPIO_PUPDR_UP,
        ._clk = GPIOG_CLK_EN
    };

    const gpio_pin_conf_t pin_green_conf = {
        ._gpioX = GPIOG,
        ._idx = 13,
        ._moder = GPIO_MODER_OUTPUT,
        ._otyper = GPIO_OTYPER_OUTPUT_PP,
        ._ospeedr = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr = GPIO_PUPDR_UP,
        ._clk = GPIOG_CLK_EN
    };

    gpio_pin_hdl_ctor(&__LED_GREEN__, &pin_green_conf);
    gpio_pin_hdl_ctor(&__LED_RED__, &pin_red_conf);
}

void __leds_fini__()
{
    gpio_pin_hdl_dtor(&__LED_GREEN__);
    gpio_pin_hdl_dtor(&__LED_RED__);
}