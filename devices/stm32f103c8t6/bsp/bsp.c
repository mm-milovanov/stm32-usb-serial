#include <bsp/bsp.h>
#include <bsp/leds.h>
#include <bsp/usart1.h>
#include <bsp/spi5.h>

#include <system/exchange.h>
#include <system/spi_control.h>

#include <lib-stm32drv/rcc.h>
#include <lib-stm32drv/systick.h>

#include <cmsis/stm32f4xx.h>

void __bsp_init__()
{
    __disable_irq();
    __rcc_init__();
    __systick_init__();
    __leds_init__();
    USART1_init();
    SPI5_init();
    __enable_irq();
}

void __bsp_fini__() {
    /* SysTick deinit */
	CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}

/* Exchange weak functions define */

void Exchange_addCallback(void* hdl, void (*callback)(void*, uint8_t)) {
	uart_hdl_add_callback(&__USART1__, hdl, callback);
}

void Exchange_transmit(exchange_hdl_t* hdl, char* data, int size) {
    USART1_transmit(data, size);
}

/* SpiControl weak functions define */

void SpiControl_write(spi_control_hdl_t* hdl, int size) {
    SPI5_write(hdl->requestBuf, size);
}

void SpiControl_read(spi_control_hdl_t* hdl, int size) {
    SPI5_read(hdl->requestBuf, hdl->responseBuf, size);
}