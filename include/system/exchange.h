#ifndef __exchange_h__
#define __exchange_h__

#include <proto/parser.h>

#define MAX_CMD_MAP_SIZE            16
#define PARSER_DEQUE_BUFFER_SIZE    128
#define PARSER_MESSAGE_BUFFER_SIZE  512

typedef struct cmd_t {
    uint8_t code;                         // Cmd code
    void* uptr;                           // Pointer to struct
    void (*exec)(void* uptr, char*, int); // Process function
} cmd_t;

typedef struct exchange_hdl_t {
    // Deque buffer
    uint8_t dequeBuf[PARSER_DEQUE_BUFFER_SIZE];
    // Recive message buffer
    uint8_t rxMsgBuf[PARSER_MESSAGE_BUFFER_SIZE];
    // Transmit message buffer
    uint8_t txMsgBuf[PARSER_MESSAGE_BUFFER_SIZE];

    // Commands map
    cmd_t cmdsMap[MAX_CMD_MAP_SIZE];
    // Size of commands map
    int cmdsMapSize;

    // Parser structure
    parser_hdl_t parser;

} exchange_hdl_t;

void Exchange_init(exchange_hdl_t* hdl);

void Exchange_step(exchange_hdl_t* hdl);

void Exchange_transmit(exchange_hdl_t* hdl, char* data, int size);

void Exchange_addCallback(void* hdl, void (*cb)(void*, uint8_t));

void Exchange_sendStatusPkg(exchange_hdl_t* hdl,
                            proto_pkg_t* pkg,
                            uint16_t status);

void Exchange_sendPkg(exchange_hdl_t* hdl,
                      uint16_t reg1,
                      uint16_t reg2,
                      uint16_t reg3,
                      char* data,
                      int size            );

#endif /* __exchange_h__ */