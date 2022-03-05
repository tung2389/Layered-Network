// l4.c

#include<arpa/inet.h>
#include<string.h>  
#include<stdio.h>

#define MAX_LENGTH 1024

// Interface to Layer 3
extern int l3_read(char* buffer, int maxlength);
extern int l3_write(char* buffer, int length);

// l4 function declarations
int build_l4_packet(char* buffer, int* length, char* name, int nameLength, char* value, int valueLength);
int read_l4_packet(char* buffer, char* name, int* nameLength, char* value, int* valueLength);


int l4_read(char* name, int* nameLength, char* value, int* valueLength)
{
    if (*nameLength > MAX_LENGTH || *valueLength > MAX_LENGTH || *nameLength + *valueLength > MAX_LENGTH)
    {
        fprintf(stderr, "Error -- buffer size not equals to length\n");
        return 0;
    }

    char l4_buf[MAX_LENGTH];

    int length = l3_read(l4_buf, MAX_LENGTH);
    
    if (length == -1)
    {
        fprintf(stderr, "Error -- l3_read\n");
        return 0;
    }

    return read_l4_packet(l4_buf, name, nameLength, value, valueLength);
    
}

int l4_write(char* name, int nameLength, char* value, int valueLength)
{
    char l4_buf[MAX_LENGTH];
    int l4_length = 0;

    // Build the L4 packet
    if (build_l4_packet(l4_buf, &l4_length, name, nameLength, value, valueLength) == 0)
    {
        return 0;
    }

    return l3_write(l4_buf, l4_length);
}

// Build l4 packet
int build_l4_packet(char* buffer, int* length, char* name, int nameLength, char* value, int valueLength)
{
    if (nameLength + valueLength > MAX_LENGTH)
    {
        return 0;
    }

    // Build the packet
    uint16_t uint16_nameLength = htons((uint16_t)nameLength);
    uint16_t uint16_valueLength = htons((uint16_t)valueLength);

    memcpy(buffer, &uint16_nameLength, sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t), &uint16_valueLength, sizeof(uint16_t));
    memcpy(buffer + 2 * sizeof(uint16_t), name, nameLength);
    memcpy(buffer + 2 * sizeof(uint16_t) + nameLength, value, valueLength);

    *length = 2 * sizeof(uint16_t) + nameLength + valueLength;

    // Send the packet
    return 1;
}

int read_l4_packet(char* buffer, char* name, int* nameLength, char* value, int* valueLength)
{
    // if (*nameLength > MAX_LENGTH || *valueLength > MAX_LENGTH || *nameLength + *valueLength > MAX_LENGTH)
    // {
    //     return 0;
    // }

    // Read the packet
    uint16_t uint16_nameLength;
    uint16_t uint16_valueLength;

    memcpy(&uint16_nameLength, buffer, sizeof(uint16_t));
    memcpy(&uint16_valueLength, buffer + sizeof(uint16_t), sizeof(uint16_t));

    *nameLength = (int)ntohs(uint16_nameLength);
    *valueLength = (int)ntohs(uint16_valueLength);

    memcpy(name, buffer + 2 * sizeof(uint16_t), *nameLength);
    memcpy(value, buffer + 2 * sizeof(uint16_t) + *nameLength, *valueLength);

    return 1;
}
