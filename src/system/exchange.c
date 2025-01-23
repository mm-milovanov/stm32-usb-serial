#include <system/exchange.h>
#include <proto/proto.h>
#include <proto/crc16.h>

#include <lib-stm32drv/uart.h>
#include <lib-stm32drv/gpio.h>

#include <cmsis/stm32f4xx.h>

// Package header and footer from control device to board
#define FROM_CTRL_PACKAGE_HEADER    ((uint16_t)0xCDE6)
#define FROM_CTRL_PACKAGE_FOOTER    ((uint16_t)0xDEC6)

// Package header and footer from board to control device
#define FROM_BOARD_PACKAGE_HEADER   ((uint16_t)0xAFE6)
#define FROM_BOARD_PACKAGE_FOOTER   ((uint16_t)0xFEA6)

/* Forward declaration of static */
// Callbacks
static void uartOnRxIrq (void* hdl, uint8_t data);
static void onValidMsg  (void* hdl, uint8_t* msg, int size);

void Exchange_init(exchange_hdl_t* hdl) {
    // Set init parser struct
    hdl->parser.init = (parser_init_t) {
        .dequeBuf = &hdl->dequeBuf,
        .dequeBufSize = PARSER_DEQUE_BUFFER_SIZE,
        .msgBuf = &hdl->rxMsgBuf,
        .msgBufSize = PARSER_MESSAGE_BUFFER_SIZE,
        .lengthOffset = -1,
        .uptr = (void*)hdl,
        .onValidMsgCallback = &onValidMsg
    };

    // Preset fix size of the proro package
    hdl->parser.status.msgSize = sizeof(proto_pkg_t);

    // Set proto struct
    hdl->parser.proto = (proto_hdl_t) {
        .ctrlHeader = FROM_CTRL_PACKAGE_HEADER,
        .ctrlFooter = FROM_CTRL_PACKAGE_FOOTER,
        .boardHeader = FROM_BOARD_PACKAGE_HEADER,
        .boardFooter = FROM_BOARD_PACKAGE_FOOTER,
        .crcSize = PROTO_CRC_SIZE_16_BIT,
        .prevMsgCount = -1
    };

    // Init parser
    Parser_init(&hdl->parser);

    // Add exchange callback for receiving bytes by USART
    Exchange_addCallback((void*)hdl, &uartOnRxIrq);
}

void Exchange_step(exchange_hdl_t* hdl) {
    Parser_step(&hdl->parser);
}

void Exchange_sendPkg(exchange_hdl_t* hdl,
                      uint16_t reg1,
                      uint16_t reg2,
                      uint16_t reg3,
                      char* data,
                      int size            )
{
    if (size > PROTO_PKG_PAYLOAD_SIZE)
        return;

    // Get pointer to start of tx msg buffer
    proto_pkg_t* pkg = &hdl->txMsgBuf;
    int pkgSize = sizeof(proto_pkg_t);

    pkg->header = FROM_BOARD_PACKAGE_HEADER;
    pkg->reg1 = reg1;
    pkg->reg2 = reg2;
    pkg->reg3 = reg3;

    for (int i=0; i<size; i++)
        pkg->payload[i] = data[i];

    pkg->cnt = 0;
    pkg->crc = get_crc16_table(pkg, pkgSize-4);
    pkg->footer = FROM_BOARD_PACKAGE_FOOTER;

    Exchange_transmit(hdl, pkg, pkgSize);
}

void Exchange_sendStatusPkg(exchange_hdl_t* hdl,
                            proto_pkg_t* pkg,
                            uint16_t status)
{
    Exchange_sendPkg(hdl, pkg->reg1, pkg->reg2, status, NULL, 0);
}

__WEAK void Exchange_transmit(exchange_hdl_t* hdl, char* data, int size) {
    /* Must be umplemented in BSP code */
}

__WEAK void Exchange_addCallback(void* hdl, void (*cb)(void*, uint8_t)) {
    /* Must be umplemented in BSP code */
}

void uartOnRxIrq(void* hdl, uint8_t data){
    exchange_hdl_t* h = (exchange_hdl_t*)hdl;
    Parser_pushByte(&h->parser, data);
}

void onValidMsg(void* uptr, uint8_t* msg, int size) {
    exchange_hdl_t* hdl = (exchange_hdl_t*)uptr;
    proto_pkg_t* pkg = msg;

    /* Get cmd index in cmd map*/
    int index = -1;
    for (int i=0; i< hdl->cmdsMapSize; i++) {       
        if (hdl->cmdsMap[i].code == pkg->reg1) {
            index = i;
            break;
        }
    }

    /* If cmd did not find then send error response and return */
    if (index == -1) {
        Exchange_sendStatusPkg(hdl, 0xFF, 0x00);
        return;
    }

    cmd_t* cmd = &hdl->cmdsMap[index];
    if (cmd->exec != NULL)
        cmd->exec(cmd->uptr, pkg, size);
}