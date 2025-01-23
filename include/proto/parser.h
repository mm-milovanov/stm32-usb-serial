#ifndef __sparser_h__
#define __sparser_h__

#include <proto/proto.h>
#include <proto/ringbuffer.h>

#include <stdint.h>
#include <stdbool.h>

typedef enum parser_state_e {
    SEARCH_HEADER = 0,
    SEARCH_FOOTER
} parser_state_e;

/* Parser init struct */
typedef struct parser_init_t {
    /* Deque */
    ring_buffer_t ringBuffer;   // ring buffer struct (deque implementation)
    uint8_t* dequeBuf;          // pointer to data buffer for deque
    uint16_t dequeBufSize;      // deque data buffer size

    /* Msg buffer */
    uint8_t* msgBuf;            // msg data buffer for rx message
    uint16_t msgBufSize;        // msg data buffer size

    /* Package parameters */
    uint16_t lengthOffset;      // offset of length word in package

    /* callbacks */
    // Handle of callback struct
    void* uptr;
    // Calls when rx msg parsed correctly
    void (*onValidMsgCallback) (void* hdl, uint8_t* buf, int len);
} parser_init_t;

/* Parser status struct */
typedef struct parser_status_t {
    uint16_t                    msgPos;
    uint16_t                    msgSize;
    parser_state_e  state; 
} parser_status_t;

typedef struct parser_hdl_t {
    proto_hdl_t     proto;
    parser_init_t   init;
    parser_status_t status;      
} parser_hdl_t;

void Parser_init      (parser_hdl_t* hdl);

void Parser_deinit    (parser_hdl_t* hdl);

void Parser_reset     (parser_hdl_t* hdl);

void Parser_pushByte  (parser_hdl_t* hdl, uint8_t byte);

void Parser_step      (parser_hdl_t* hdl);

#endif  // __sparser_h__
