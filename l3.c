#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// Interface to Layer 2
extern int l2_read(char* buffer, int maxlength);
extern int l2_write(char* buffer, int length);

typedef uint32_t crc_t;

#define CRC_POLY 0x04C11DB7
#define WIDTH (8 * sizeof(crc_t))
#define LEFT_MOST (1 << (WIDTH - 1))

crc_t crc_table[256];

bool check_crc(char *buffer);
void init_crc_table();
crc_t calc_crc(char *buffer, int length);
void l3_init();

int l3_read(char* buffer, int maxlength) {

}

int l3_write(char* buffer, int length) {

}

void l3_init() {
    init_crc_table();
}

void init_crc_table() {
    for(int dividend = 0; dividend < 256; dividend++) {
        crc_t remainder = ((crc_t) dividend << (WIDTH - 8));
        for(int bit = 0; bit < 8; bit++) {
            if((remainder & LEFT_MOST) == LEFT_MOST) {
                remainder <<= 1;
                remainder ^= CRC_POLY;
            }
            else {
                remainder <<= 1;
            }
        }
        crc_table[dividend] = remainder;
        // printf("%x\n", crc_table[dividend]);
    }
}

crc_t calc_crc(char *buffer, int length) {
    crc_t crc = 0;
    for(int byte = 0; byte < length; byte++) {
        uint8_t cur_byte = buffer[byte];
        uint8_t pos = (uint8_t)((crc ^ (cur_byte << (WIDTH - 8))) >> (WIDTH - 8));
        crc = (crc_t) ((crc << 8) ^ (crc_t) (crc_table[pos]));
    }
    return crc;
}