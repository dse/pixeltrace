#include "endian.h"

uint32_t le32toh(uint32_t n) {
    unsigned char *np = (unsigned char *)&n;
    return ((uint32_t)np[3] << 24) |
         ((uint32_t)np[2] << 16) |
         ((uint32_t)np[1] << 8) |
         (uint32_t)np[0];
}

uint16_t le16toh(uint16_t n) {
    unsigned char *np = (unsigned char *)&n;
    return ((uint16_t)np[1] << 8) | (uint16_t)np[0];
}
