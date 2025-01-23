#include <bsp/usart1.h>

#include <defconfig.h>

#include <lib-stm32drv/uart.h>
#include <lib-stm32drv/gpio.h>

#include <cmsis/stm32f4xx.h>

#define USART1_CLK_EN 0x00000010

uart_hdl_t __USART1__;

static gpio_pin_hdl_t rxPin;
static gpio_pin_hdl_t txPin;

void USART1_IRQHandler() {
    uart_hdl_on_irq(&__USART1__);
}

void USART1_init() {
    /* init rx pin */
    gpio_pin_conf_t txPinConf = {
        ._gpioX = GPIOA,
        ._idx = 9,
        ._moder = GPIO_MODER_ALTERNATE,
        ._ospeedr = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr = GPIO_PUPDR_NO,
        ._afr = 0x7,
        ._clk = GPIOA_CLK_EN,
    };
    gpio_pin_hdl_ctor(&txPin, &txPinConf);

    /* init tx pin */
    gpio_pin_conf_t rxPinConf = {
        ._gpioX = GPIOA,
        ._idx = 10,
        ._moder = GPIO_MODER_ALTERNATE,
        ._ospeedr = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr = GPIO_PUPDR_NO,
        ._afr = 0x7,
        ._clk = GPIOA_CLK_EN
    };
    gpio_pin_hdl_ctor(&rxPin, &rxPinConf);

    /* init usart */
    uart_conf_t _uart_conf = {
        ._usartX = USART1,
        ._baud_rate = 921600,
        ._word_length = USART_WORD_LENGTH_8B,
        ._stop_bits = USART_STOP_BITS_1,
        ._parity = USART_PARITY_DISABLE,

        ._clk_src = CLK_SRC_APB2,
        ._clk_en = USART1_CLK_EN,

        ._irq = USART1_IRQn,
    };    
    uart_hdl_ctor(&__USART1__, &_uart_conf);
}

void USART1_deinit() {
    /* disable interrupts */
    CLEAR_BIT(USART3->CR1, USART_CR1_RXNEIE);
    NVIC_DisableIRQ(USART3_IRQn);
}

void USART1_transmit(uint8_t* buf, int size) {
    uart_hdl_tx(&__USART1__, buf, size);
}