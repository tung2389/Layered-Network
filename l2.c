// l2.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LENGTH 1024

// Interface to Layer 1 
extern int l1_read(char* b);
extern int l1_write(char b);

// l2 function declarations
int build_l2_packet(char* l2_buf, int* l2_length, char* buffer, int length);
int read_l2_packet(char* buffer, int* maxlength, char* length_buf);

int l2_read(char* buffer, int maxlength)
{
    char length_buf[2];

    // Read the length
    for (int i = 0; i < 2; i++)
    {
        if (l1_read(&length_buf[i]) == -1)
        {
            perror("l1_read");
            return 0;
        }
    }

    if (read_l2_packet(buffer, &maxlength, length_buf) == 0)
    {
        return -1;
    }

    return maxlength;
}

int l2_write(char* buffer, int length)
{
    char l2_buf[MAX_LENGTH];
    int l2_length = 0;

    // Build the L2 packet
    if (build_l2_packet(l2_buf, &l2_length, buffer, length) == 0)
    {
        return -1;
    }

    // output packet
    for (int i = 0; i < l2_length; i++)
    {
        if (l1_write(l2_buf[i]) == -1)
        {
            perror("l1_write");
            return -1;
        };
    }

    return l2_length;
}

int build_l2_packet(char* l2_buf, int* l2_length, char* buffer, int length)
{
    if (length + 2 > MAX_LENGTH)
    {
        fprintf(stderr, "Error -- buffer size not equals to length\n");
        return 0;
    }

    uint16_t uint16_length = htons((uint16_t)length);

    memcpy(l2_buf, &uint16_length, sizeof(uint16_t));
    memcpy(l2_buf + sizeof(uint16_t), buffer, length);

    *l2_length = length + 2;

    return 1;
}

int read_l2_packet(char* buffer, int* maxlength, char* length_buf)
{
    if (*maxlength > MAX_LENGTH)
    {
        return 0;
    }

    // Convert the length
    uint16_t uint16_length;
    memcpy(&uint16_length, length_buf, sizeof(uint16_t));
    int length = (int)ntohs(uint16_length);

    if (length > *maxlength)
    {
        fprintf(stderr, "l2 Error -- buffer size exceeds length\n");
        return 0;
    }

    // Read the packet
    for (int i = 0; i < length; i++)
    {
        if (l1_read(&buffer[i]) == -1)
        {
            perror("l1_read");
            return 0;
        }
    }

    *maxlength = length;

    return 1;
}