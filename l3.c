#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

// Interface to Layer 2
extern int l2_read(char* buffer, int maxlength);
extern int l2_write(char* buffer, int length);

typedef uint32_t crc_t;

#define CRC_POLY 0x04C11DB7
#define WIDTH (8 * sizeof(crc_t))
#define LEFT_MOST (1 << (WIDTH - 1))

// This variable is true if the crc_table has been initialized, false otherwise.
bool init_crc = false;
crc_t crc_table[256];

bool check_crc(char *buffer, int length);
void init_crc_table();
crc_t calc_crc(char *buffer, int length);

int l3_read(char* buffer, int maxlength) {
    if(!init_crc) {
        init_crc_table();
        init_crc = true;
    }
    
    if(maxlength < sizeof(crc_t)) return -1;

    int l3_max_length = maxlength + sizeof(crc_t);
    char l3_buf[l3_max_length];
    int length = l2_read(l3_buf, l3_max_length);

    if(length == -1) return -1;
    // Check crc
    if(!check_crc(l3_buf, length)) return -1;

    // Strip the crc
    memcpy(buffer, l3_buf, length - sizeof(crc_t));
    return length - sizeof(crc_t);
}

int l3_write(char* buffer, int length) {
    if(!init_crc) {
        init_crc_table();
        init_crc = true;
    }
    
    // Create new buffer with length of crc + length of buffer from upper layer.
    int l3_length = length + sizeof(crc_t);
    char l3_buf[l3_length];

    // Create CRC for the buffer from upper layer
    crc_t crc = calc_crc(buffer, length);
    crc = htonl(crc);
    
    // Copy the CRC and the old buffer to the new buffer
    memcpy(l3_buf, buffer, length);
    memcpy(l3_buf + length, &crc, sizeof(crc_t));
    
    return l2_write(l3_buf, l3_length);
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

bool check_crc(char *buffer, int length) {
    return (calc_crc(buffer, length) == 0);
}