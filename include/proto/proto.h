#ifndef __proto_h__
#define __proto_h__

#include <stdint.h>
#include <stdbool.h>

#define PROTO_PKG_PAYLOAD_SIZE 128

; /* to fix clang warning */
#pragma pack(push,1)

typedef struct proto_pkg_t {
    // Header
    uint16_t header;
    // Registers
    uint16_t reg1;
    uint16_t reg2;
    uint16_t reg3;
    // Payload
    char payload[PROTO_PKG_PAYLOAD_SIZE];
    // Control
    uint16_t cnt;
    uint16_t crc;
    // Footer
    uint16_t footer;
} proto_pkg_t;

#pragma pack(pop)

typedef enum proto_crc_size_e {
    PROTO_CRC_SIZE_16_BIT = 2,
    PROTO_CRC_SIZE_32_BIT = 4
} proto_crc_size_e;

typedef enum proto_msg_status_e {
    PROTO_MSG_STATUS_VALID           =  0,
    PROTO_MSG_STATUS_INVALID_SIZE    = -1,
    PROTO_MSG_STATUS_INVALID_HEADER  = -2,
    PROTO_MSG_STATUS_INVALID_FOOTER  = -3,
    PROTO_MSG_STATUS_INVALID_CRC     = -4
} proto_msg_status_e;

typedef struct proto_hdl_t {
    uint16_t ctrlHeader;        // Package header from control device to board
    uint16_t ctrlFooter;        // Package footer from control device to board
    uint16_t boardHeader;       // Package header from board to control device
    uint16_t boardFooter;       // Package footer from board to control device
    uint16_t prevMsgCount;      // Previous message count value
    proto_crc_size_e crcSize;   // Package control sum size
} proto_hdl_t;

bool Proto_checkRxMsgHeader(proto_hdl_t* hdl, uint16_t header);

bool Proto_checkRxMsgFooter(proto_hdl_t* hdl, uint16_t footer);

proto_msg_status_e Proto_checkRxMsg(proto_hdl_t* hdl, uint8_t* data, int len);

#endif /* __proto_h__ */