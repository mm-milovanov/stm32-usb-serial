#ifndef __crc16_t__
#define __crc16_t__

#include <stdint.h>

uint16_t get_crc16_table(const void *buf, uint16_t length_in_bytes);

#endif /* __crc16_t__ */