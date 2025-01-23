#include <proto/proto.h>
#include <proto/crc16.h>

/* Static function forward delcatation */
static uint16_t get16BitWord(uint8_t* data, int pos);

bool Proto_checkRxMsgHeader(proto_hdl_t* hdl, uint16_t header) {
    return (hdl->ctrlHeader == header);
}

bool Proto_checkRxMsgFooter(proto_hdl_t* hdl, uint16_t footer) {
    return (hdl->ctrlFooter == footer);
}

proto_msg_status_e Proto_checkRxMsg(proto_hdl_t* hdl, uint8_t* data, int len)
{
    proto_pkg_t* pkg = (proto_pkg_t*) data;

    // Check message size
    if (len != sizeof(proto_pkg_t))
        return PROTO_MSG_STATUS_INVALID_SIZE;

    // Check message header
    if (!Proto_checkRxMsgHeader(hdl, pkg->header))
        return PROTO_MSG_STATUS_INVALID_HEADER;

    // Check message footer
    if (!Proto_checkRxMsgFooter(hdl, pkg->footer))
        return PROTO_MSG_STATUS_INVALID_FOOTER;

    // Check message control sum
    if (get_crc16_table(data, len-4) != pkg->crc)
        return PROTO_MSG_STATUS_INVALID_CRC;

    // If msg passes all 'if's then return valid status
    return PROTO_MSG_STATUS_VALID;
}

uint16_t get16BitWord(uint8_t* data, int pos) {
    return (((uint16_t)data[pos+1]) << 8) | data[pos];
}