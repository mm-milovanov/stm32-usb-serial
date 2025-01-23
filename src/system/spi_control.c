#include <system/spi_control.h>
#include <system/exchange.h>
#include <proto/proto.h>

#include <cmsis/stm32f4xx.h>

#include <stddef.h>

void SpiControl_init(spi_control_hdl_t* hdl) {
    // Get info about spi module
    // SpiControl_setUp(hdl); 
    SpiControl_clearBufs(hdl);
}

// int SpiControl_getVar(spi_control_hdl_t*    hdl,
//                       char*                 data,
//                       int                   size)
// {
//     proto_pkg_t* pkg = data;

//     /* Get cmd index in cmd map*/
//     int index = -1;
//     for (int i=0; i< hdl->mapSize; i++) {       
//         if (hdl->spiVarMap[i].code == pkg->reg2) {
//             index = i;
//             break;
//         }
//     }

//     /* If cmd did not find then send error response and return */
//     if (index == -1) {
//         Exchange_sendStatusPkg(hdl, pkg, 0xFFFF);
//         return;
//     }

//     /* Execute cmd */
//     spi_var_t* cmd = &hdl->spiVarMap[index];

//     return (cmd->getVal != NULL) ? cmd->getVal() : -1;
// }

// bool SpiControl_setVar(spi_control_hdl_t*    hdl,
//                       char*                  data,
//                       int                    size)
// {
//     proto_pkg_t* pkg = data;
//     uint32_t value = ((uint32_t*)&pkg->payload)[0];

//     /* Get cmd index in cmd map*/
//     int index = -1;
//     for (int i=0; i< hdl->mapSize; i++) {       
//         if (hdl->spiVarMap[i].code == pkg->reg2) {
//             index = i;
//             break;
//         }
//     }

//     /* If cmd did not find then send error response and return */
//     if (index == -1) {
//         Exchange_sendStatusPkg(hdl, pkg, 0xFFFF);
//         return;
//     }

//     /* Execute cmd */
//     spi_var_t* cmd = &hdl->spiVarMap[index];

//     return (cmd->setVal != NULL) ? cmd->setVal(value) : false;
// }

// __WEAK void SpiControl_setUp(spi_control_hdl_t* hdl) {
//     /* Must be implemented in BSP code */
// }

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