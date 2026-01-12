#include <system/spi_control.h>
#include <system/exchange.h>
#include <proto/proto.h>

#include <cmsis/gcc.h>

#include <stddef.h>

void SpiControl_init(spi_control_hdl_t* hdl) {
    // Get info about spi module
    // SpiControl_setUp(hdl); 
    SpiControl_clearBufs(hdl);
}

__WEAK void SpiControl_write(spi_control_hdl_t* hdl, int size) {
    /* Must be implemented in BSP code */
}

__WEAK void SpiControl_read(spi_control_hdl_t* hdl, int size) {
    /* Must be implemented in BSP code */
}

void SpiControl_clearBufs(spi_control_hdl_t* hdl) {
    for (int i=0; i<SPI_BUF_SIZE; i++) {
        hdl->responseBuf[i] = 0x0;
        hdl->requestBuf[i] = 0x0;
    }
}