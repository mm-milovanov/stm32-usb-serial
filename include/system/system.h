#ifndef __system_h__
#define __system_h__

#include <system/exchange.h>
#include <system/spi_control.h>

typedef struct system_hdl_t {
    spi_control_hdl_t spiControl;
    exchange_hdl_t exchange;
} system_hdl_t;

void System_init(system_hdl_t* hdl);

void System_step(system_hdl_t* hdl);

// System exchange calls

void System_onEcho(exchange_hdl_t* hdl, char* data, int size);

void System_getVar(system_hdl_t* hdl, char* data, int size);

void System_spiWrite(system_hdl_t* hdl, char* data, int size);

void System_spiRead(system_hdl_t* hdl, char* data, int size);

#endif /* __system_h__ */