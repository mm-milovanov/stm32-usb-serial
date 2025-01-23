#include <proto/parser.h>
#include <proto/proto.h>

/* Static function forward declaration */
static inline bool checkHeader      (parser_hdl_t* hdl);
static inline bool checkFooter      (parser_hdl_t* hdl);
static inline void resetParser      (parser_hdl_t* hdl);
static inline bool getData          (parser_hdl_t* hdl);
static inline void searchHeaderStep (parser_hdl_t* hdl);
static inline void searchFooterStep (parser_hdl_t* hdl);
static inline void onValidFooter    (parser_hdl_t* hdl);

void Parser_init(parser_hdl_t* hdl)
{
    assert(hdl);

    hdl->status.msgPos = 0;
    hdl->status.state = SEARCH_HEADER;

    // Init ring buffer
    ring_buffer_init(
        &hdl->init.ringBuffer, 
        (char*)hdl->init.dequeBuf,
        hdl->init.dequeBufSize
    );

    // Reset parser state
    resetParser(hdl);
}

void Parser_deinit(parser_hdl_t* hdl) {
    /* Unused */
}

void Parser_reset(parser_hdl_t* hdl) {
    resetParser(hdl);
}

void Parser_pushByte(parser_hdl_t* hdl,
                     uint8_t       byte)
{
    assert(hdl);
    ring_buffer_queue(&hdl->init.ringBuffer, byte);
}

void Parser_step(parser_hdl_t* hdl)
{
    while (getData(hdl))
    {
        switch (hdl->status.state)
        {
            case SEARCH_HEADER:
                searchHeaderStep(hdl);
                break;
            case SEARCH_FOOTER:
                searchFooterStep(hdl);
                break;
        }
        // Increase possition in msg
        hdl->status.msgPos++;
    }
}

/* Define static function */

bool getData(parser_hdl_t* hdl) {
    ring_buffer_t* ringBuffer = &hdl->init.ringBuffer;
    uint8_t* msgBuf = hdl->init.msgBuf;
    int msgPos = hdl->status.msgPos;

    return (ring_buffer_dequeue(ringBuffer, &msgBuf[msgPos]) == 1);
}

void onValidFooter(parser_hdl_t* hdl) {
    // Get pointers
    uint8_t* data = hdl->init.msgBuf;
    int len = hdl->status.msgPos+1;

    // Check message
    proto_msg_status_e status
        = Proto_checkRxMsg(&hdl->proto, data, len);
    if (status != PROTO_MSG_STATUS_VALID)
        return;

    // If msg valid call on valid message callback
    if (hdl->init.onValidMsgCallback != NULL)
        // Pass only cmd data from 4 to msgSize-6 bytes
        hdl->init.onValidMsgCallback(hdl->init.uptr, data, len);
}

void searchHeaderStep(parser_hdl_t* hdl)
{
    uint8_t* msgBuf = hdl->init.msgBuf;

    // Need 2 bytes data for header
    if (hdl->status.msgPos < 1)
        return;

    if (checkHeader(hdl))
        hdl->status.state = SEARCH_FOOTER;
    else {
        msgBuf[0] = msgBuf[1];
        hdl->status.msgPos = 0;
    }
}

void searchFooterStep(parser_hdl_t* hdl)
{
    // Get pointers
    parser_init_t* init = &hdl->init;
    parser_status_t* status = &hdl->status;
    uint8_t* msgBuf = hdl->init.msgBuf;
    // Get possition in msg
    int pos = status->msgPos;

    /* Check that possition pass msg size */
    if (pos == init->msgBufSize-1) {
        resetParser(hdl);
        return;
    }

    if (pos == init->lengthOffset+1)
        status->msgSize = (msgBuf[pos] << 8) | msgBuf[pos-1];

    if ((pos == status->msgSize-1) && checkFooter(hdl)) {
        onValidFooter(hdl);
        resetParser(hdl);
    }
}

bool checkHeader(parser_hdl_t* hdl) {
    uint16_t header = ((uint16_t*)hdl->init.msgBuf)[0];
    Proto_checkRxMsgHeader(&hdl->proto, header);
}

bool checkFooter(parser_hdl_t* hdl) {
    uint8_t* msgBuf = hdl->init.msgBuf;
    int pos = hdl->status.msgPos;

    uint16_t footer = (msgBuf[pos] << 8) | msgBuf[pos-1];
    Proto_checkRxMsgFooter(&hdl->proto, footer);
}

void resetParser(parser_hdl_t* hdl) {
    hdl->status.state = SEARCH_HEADER;
    hdl->status.msgPos = 0;
}