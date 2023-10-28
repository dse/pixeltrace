#ifndef ENDIAN_H
#define ENDIAN_H

/* h/t https://stackoverflow.com/a/2103095 */

#include <limits.h>
#include <stdint.h>

enum {
     PT_LITTLE_ENDIAN    = 0x03020100ul,
     PT_BIG_ENDIAN       = 0x00010203ul,
     PT_PDP_ENDIAN       = 0x01000302ul,
     PT_HONEYWELL_ENDIAN = 0x02030001ul
};

static const union {
     unsigned char bytes[4];
     uint32_t value;
} host_order = { { 0, 1, 2, 3 } };

#define HOST_ORDER (host_order.value)

uint32_t le32toh(uint32_t from);
uint16_t le16toh(uint16_t from);

#endif
