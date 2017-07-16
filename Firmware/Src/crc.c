#include <stdint.h>

// This snippit adapted from http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html

//TODO use HW block
uint8_t crc_CRC8(uint8_t const * const buf, int len) {
    const uint8_t generator = 0x1D;
    uint8_t crc = 0; /* start with 0 so first byte can be 'xored' in */

    int j;
    for(j = 0; j < len; j++) {
        crc ^= buf[j];

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x80) != 0)
            {
                crc = (uint8_t)((crc << 1) ^ generator);
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

