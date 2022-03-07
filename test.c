#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef uint32_t crc_t;

// l1 function declarations
extern crc_t crc_table[256];
extern void init_crc_table();
extern crc_t calc_crc(char *buffer, int length);
extern bool check_crc(char *buffer, int length);

// l2 function declarations
extern int build_l4_packet(char* buffer, int* length, char* name, int nameLength, char* value, int valueLength);
extern int read_l4_packet(char* buffer, char* name, int* nameLength, char* value, int* valueLength);

void test_calc_crc() {
    init_crc_table();
    char *buf = "123456789";
    assert(calc_crc(buf, strlen(buf)) == 0x89A1897F);
    buf = "fsdjkfsdafdsdsf";
    assert(calc_crc(buf, strlen(buf)) == 0xC664EABF);
    buf = "ruerweojdcdnsci212332";
    assert(calc_crc(buf, strlen(buf)) == 0xF27C4D33);
    printf("Test calc_crc passed!\n");
}

void test_check_crc() {
    init_crc_table();
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

void test_build_l4_packet() {
    char buf[13];
    int length;
    
    assert(build_l4_packet(buf, &length, "name", 4, "value", 5) == 1);

    assert(length == 13);

    char buf2[13] = {0x00, 0x04, 0x00, 0x05, 'n', 'a', 'm', 'e', 'v', 'a', 'l', 'u', 'e'};
    assert(strcmp(buf, buf2) == 0);

    printf("Test build_l4_packet passed!\n");
}

void test_read_l4_packet() {
    char buf[13] = {0x00, 0x04, 0x00, 0x05, 'n', 'a', 'm', 'e', 'v', 'a', 'l', 'u', 'e'};
    char name[1024];
    memset(name, 0, 1024);
    int nameLength;
    char value[1024];
    memset(value, 0, 1024);
    int valueLength;

    assert(read_l4_packet(buf, name, &nameLength, value, &valueLength) == 1);

    assert(strcmp(name, "name") == 0);
    assert(strcmp(value, "value") == 0);

    printf("Test read_l4_packet passed!\n");
}

int main() {
    // test l3
    printf("Testing l3...\n");
    test_calc_crc();
    test_check_crc();
    printf("\n");

    // test l4
    printf("Testing l4...\n");
    test_build_l4_packet();
    test_read_l4_packet();
}