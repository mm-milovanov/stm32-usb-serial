#include "bsp.h"
#include "led.h"
#include "spi1.h"

#include <system/spi_control.h>
#include <system/exchange.h>

#include <ESDL/rcc.h>
#include <cortex/systick.h>
#include <cmsis/stm32f1xx.h>

void Bsp_init()
{
    __disable_irq();
    Rcc_init();
    SysTick_init(Rcc_getCoreClkFreq());
    Led_init();
    SPI1_init();
    USART1_init();
    __enable_irq();
}

void Bsp_deinit() {
    __disable_irq();
    SPI1_deinit();
    Led_deinit();
    SysTick_deinit();
    __enable_irq();
}

/* SpiControl weak functions define */

void SpiControl_write(spi_control_hdl_t* hdl, int size) {
    SPI1_write(hdl->requestBuf, size);
}

void SpiControl_read(spi_control_hdl_t* hdl, int size) {
    SPI1_read(hdl->requestBuf, hdl->responseBuf, size);
}

/* Exchange weak functions define */

void Exchange_addCallback(void* hdl, void (*callback)(void*, uint8_t)) {
	uart_hdl_add_callback(&__USART1__, hdl, callback);
}

void Exchange_transmit(exchange_hdl_t* hdl, char* data, int size) {
    USART1_transmit(data, size);
}