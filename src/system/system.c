#include <system/system.h>
#include <system/spi_control.h>

#include <cmsis/stm32f1xx.h>

#include <cortex/systick.h>

#define STR_BUF_SIZE            128
// Str buffer
static char strBuf[STR_BUF_SIZE];

void System_init(system_hdl_t* hdl) {
    /* Set cmds to cmds map */

    // Echo
    hdl->exchange.cmdsMap[0] = (cmd_t) {
        0x00,
        &hdl->exchange,
        &System_onEcho
    };

    // Spi write
    hdl->exchange.cmdsMap[1] = (cmd_t) {
        0x20,
        hdl,
        &System_spiWrite
    };

    // Spi read
    hdl->exchange.cmdsMap[2] = (cmd_t) {
        0x21,
        hdl,
        &System_spiRead
    };

    // Set size of cmdsMap
    hdl->exchange.cmdsMapSize = 3;

    Exchange_init(&hdl->exchange);
    SpiControl_init(&hdl->spiControl);
}

void System_step(system_hdl_t* hdl) {
    Exchange_step(&hdl->exchange);
}

void System_onEcho(exchange_hdl_t* hdl, char* data, int size)
{
    proto_pkg_t* pkg = data;

    /* Send 'empty' pkg to control device */
    Exchange_sendStatusPkg(hdl, pkg, 0x00);
}

void System_setVar(system_hdl_t* hdl, char* data, int size) {
    proto_pkg_t* pkg = data;

    uint16_t status = 0x0; 
    if (!SpiControl_setVar(hdl, data, size))
        status = 0xFFFF;
    Exchange_sendStatusPkg(&hdl->exchange, pkg, status);
}

void System_spiWrite(system_hdl_t* hdl, char* data, int size) {
    proto_pkg_t* pkg = data;
    uint8_t payloadSize = pkg->reg2;

    for (int i=0; i<payloadSize; i++)
        hdl->spiControl.requestBuf[i] = pkg->payload[i];

    SpiControl_write(&hdl->spiControl, payloadSize);

    Exchange_sendStatusPkg(&hdl->exchange, pkg, 0x00);

    SpiControl_clearBufs(&hdl->spiControl);
}

void System_spiRead(system_hdl_t* hdl, char* data, int size) {
    proto_pkg_t* pkg = data;
    uint8_t payloadSize = pkg->reg2;

    for (int i=0; i<payloadSize; i++)
        hdl->spiControl.requestBuf[i] = pkg->payload[i];

    SpiControl_read(&hdl->spiControl, payloadSize);

    if (hdl->spiControl.responseBuf[1] == '\xFF') {
        int tmp = 0;
        tmp++;
    }


    Exchange_sendPkg(&hdl->exchange,
                     pkg->reg1,
                     pkg->reg2,
                     0x00,
                     &hdl->spiControl.responseBuf,
                     payloadSize
    );

    SpiControl_clearBufs(&hdl->spiControl);
}