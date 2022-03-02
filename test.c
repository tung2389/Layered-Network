#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef uint32_t crc_t;

extern crc_t crc_table[256];
extern void l3_init();
extern crc_t calc_crc(char *buffer, int length);
extern bool check_crc(char *buffer, int length);

void test_calc_crc() {
    l3_init();
    char *buf = "123456789";
    assert((crc_t) calc_crc(buf, strlen(buf)) == 0x89A1897F);
    buf = "fsdjkfsdafdsdsf";
    assert((crc_t) calc_crc(buf, strlen(buf)) == 0xC664EABF);
    buf = "ruerweojdcdnsci212332";
    assert((crc_t) calc_crc(buf, strlen(buf)) == 0xF27C4D33);
    printf("Test calc_crc passed!\n");
}

void test_check_crc() {
    l3_init();
    char buf[13] = {'1','2','3','4','5','6','7','8','9', 0x89, 0xA1, 0x89, 0x7F};
    assert(check_crc(buf, 13));
    buf[4] = 6;
    assert(check_crc(buf, 13) == false); 
    buf[11] = 0x88;
    assert(check_crc(buf, 13) == false);

    char buf2[12] = {'a', 'b', 'c', 'd', '6', '4', '5', '7', 0x59, 0x82, 0x68, 0x0B};
    assert(check_crc(buf2, 12));
    
    printf("Test check_crc passed!\n");
}

int main() {
    test_calc_crc();
    test_check_crc();
}