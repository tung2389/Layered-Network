#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

typedef uint32_t crc_t;

extern crc_t crc_table[256];
extern void l3_init();
extern crc_t calc_crc(char *buffer, int length);

void test_calc_crc() {
    l3_init();
    char *buf = "123456789";
    assert((crc_t) calc_crc(buf, strlen(buf)) == 0x89A1897F);
    buf = "fsdjkfsdafdsdsf";
    assert((crc_t) calc_crc(buf, strlen(buf)) == 0xC664EABF);
    buf = "ruerweojdcdnsci212332";
    assert((crc_t) calc_crc(buf, strlen(buf)) == 0xF27C4D33);
    printf("Test calc_crc passed!");
}

int main() {
    test_calc_crc();
}