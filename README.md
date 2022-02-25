# Project 3: Layered Network Protocols
For this project, we write layered software to support one-way (half-duplex) communication between a single sender/receiver pair.

## Layer 1: Single Byte
Layer 1 provides ordered single byte communication from the sender to the receiver.

### Functions
    1. `int l1 write(char b);`

    2. `int l1 read(char* b);`

## Layer 2: Messages
Layer 2 provides the ability to send and receive a message where a message is simply a sequence of bytes with a particular format. 

### Functions
    1. `int l2 write(char* buffer, int length);` 

    2. `int l2 read(char* buffer, int maxLength);`

## Layer 3: Messages with Error Detection
Layer 3 adds simple error detection to the services provided by layer 2. 
### Functions
    1. `int l3 write(char* buffer, int length);`

    2. `int l3 read(char* buffer, int maxLength);`

## Layer 4: Name/Value Pairs
Layer 4 will provide a mechanism for sending and receiving values that have an associated
name.

### Functions
    1. `int l4 write(char *name, int nameLength, char *value, int valueLength);int l3 write(char* buffer, int length);`

    2. `int l4 read(char* name, int* nameLength, char* value, int* valueLength);int l3 read(char* buffer, int maxLength);`