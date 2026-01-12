#ifndef __spi_control_h__
#define __spi_control_h__

#include <ESDL/spi.h>

#include <stdbool.h>

#define SPI_BUF_SIZE            128
#define MAX_SPI_VAR_MAP_SIZE    16

typedef struct spi_var_t {
    uint8_t code;
    int (*getVal)();
    bool (*setVal)(uint32_t);
} spi_var_t;

typedef struct spi_control_hdl_t {
    // Request buffer
    char requestBuf[SPI_BUF_SIZE];
    // Response buffer
    char responseBuf[SPI_BUF_SIZE];

    // SPI variable map
    // spi_var_t spiVarMap[MAX_SPI_VAR_MAP_SIZE];
    // int mapSize;
} spi_control_hdl_t;

void SpiControl_init(spi_control_hdl_t* hdl);

// void SpiControl_setUp(spi_control_hdl_t* hdl);

// int SpiControl_getVar(spi_control_hdl_t*    hdl,
//                       char*                 data,
//                       int                   size);

// bool SpiControl_setVar(spi_control_hdl_t*    hdl,
//                        char*                 data,
//                        int                   size);

void SpiControl_write(spi_control_hdl_t* hdl, int size);

void SpiControl_read(spi_control_hdl_t* hdl, int size);

void SpiControl_clearBufs(spi_control_hdl_t* hdl);

#endif /* __spi_control_h__ */