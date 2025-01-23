#include "bsp.h"
#include "led.h"
// #include <bsp/spi5.h>

// #include <system/exchange.h>
// #include <system/spi_control.h>

#include <ESDL/rcc.h>
#include <cortex/systick.h>
#include <cmsis/stm32f1xx.h>

void Bsp_init()
{
    __disable_irq();
    Rcc_init();
    SysTick_init(Rcc_getCoreClkFreq());
    Led_init();
    // SPI5_init();
    __enable_irq();
}

void Bsp_deinit() {
    SysTick_deinit();
    Led_deinit();
}

/* Exchange weak functions define */

// void Exchange_addCallback(void* hdl, void (*callback)(void*, uint8_t)) {
// 	uart_hdl_add_callback(&__USART1__, hdl, callback);
// }

// void Exchange_transmit(exchange_hdl_t* hdl, char* data, int size) {
//     USART1_transmit(data, size);
// }

// /* SpiControl weak functions define */

// void SpiControl_write(spi_control_hdl_t* hdl, int size) {
//     SPI5_write(hdl->requestBuf, size);
// }

// void SpiControl_read(spi_control_hdl_t* hdl, int size) {
//     SPI5_read(hdl->requestBuf, hdl->responseBuf, size);
// }