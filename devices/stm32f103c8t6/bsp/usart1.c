#include "usart1.h"

#include <ESDL/uart.h>
#include <ESDL/gpio.h>

#include <cmsis/stm32f1xx.h>

#define USART1_CLK_EN 0x00004000

uart_hdl_t __USART1__;

static gpio_pin_hdl_t rxPin;
static gpio_pin_hdl_t txPin;

void USART1_IRQHandler() {
    uart_hdl_on_irq(&__USART1__);
}

void USART1_init() {
    /* init rx pin */
    gpio_pin_conf_t txPinConf = {
        .gpioX = GPIOA,
        .idx = 9,
        .cnf = GPIO_CNF_ALTERNATE_PUSH_PULL_OUTPUT,
        .mode = GPIO_MODE_OUTPUT_50MHZ,
        .clk = GPIOA_CLK_EN,
    };
    gpio_pin_hdl_ctor(&txPin, &txPinConf);

    /* init tx pin */
    gpio_pin_conf_t rxPinConf = {
        .gpioX = GPIOA,
        .idx = 10,
        .cnf = GPIO_CNF_FLOATING_INPUT,
        .mode = GPIO_MODE_INPUT,
        .clk = GPIOA_CLK_EN
    };
    gpio_pin_hdl_ctor(&rxPin, &rxPinConf);

    /* init usart */
    uart_conf_t _uart_conf = {
        ._usartX = USART1,
        ._baud_rate = 115200,
        ._word_length = USART_WORD_LENGTH_8B,
        ._stop_bits = USART_STOP_BITS_1,
        ._parity = USART_PARITY_DISABLE,

        ._clk_src = 1,
        ._clk_en = USART1_CLK_EN,

        ._irq = USART1_IRQn,
    };    
    uart_hdl_ctor(&__USART1__, &_uart_conf);
}

void USART1_deinit() {
    /* disable interrupts */
    CLEAR_BIT(USART1->CR1, USART_CR1_RXNEIE);
    NVIC_DisableIRQ(USART1_IRQn);
}

void USART1_transmit(uint8_t* buf, int size) {
    uart_hdl_tx(&__USART1__, buf, size);
}