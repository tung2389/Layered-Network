# Layered Network
This is an implementation of a layered network, facilitating half-duplex communications between two peers, a sender and a receiver.

## Layer 1: Single Byte
At the lowest level, the communication is done via UNIX pipe. This layer simply provides an interface to read a single byte to STDIN and write a single byte to STDOUT.

```c
int l1_write(char b);
```
Writes the byte specified by b. Returns 1 (true) on success or 0 (false) on error.

```c
int l1_read(char* b);
```
Reads one byte and copies the byte to the address specified by b. Returns 1 (true) on
success or 0 (false) on error.

## Layer 2: Messages
Layer 2 provides the ability to send and receive a message. A message is simply a sequence of bytes with a particular format. 

```c
int l2_write(char* buffer, int length);
```
Sends a message consisting of the sequence of bytes starting at the address in buffer
and having length length. Returns length on success or -1 on an error

```c
int l2_read(char* buffer, int maxLength);
```
Reads a message and stores it at the address specified by buffer. No more than
maxlength bytes will be put into memory. If a message received by l2_read would
require more than maxLength bytes, l2_read return -1. Upon successful reception of a message, the size of the message (the number of bytes actually stored in buffer) is returned.

## Layer 3: Messages with Error Detection
Layer 3 adds error detection to the services provided by layer 2. The service interface
for layer 3 looks the same as the layer 2 service interface, but the layer 3 read also
return a -1 if it detects an error in the received message. Layer 3 uses a CRC attached to each message to check if an error has occured or not. 

```c
int l3_write(char* buffer, int length);
```
Sends a message that consists of the sequence of bytes starting at the address specified
by buffer and having length length. Returns length on success or -1 on error.

```c
int l3_read(char* buffer, int maxLength);
```
Reads a message and stores it starting at the address specified by buffer. No more
than maxLength bytes will be put into memory. If a message is received by l3_read
that would require more than maxLength bytes, l3_read return -1. If some errors are detected in the message, l3_read return -1. Upon successful reception of a message, the size of the message (the number of bytes stored in buffer) is returned.

### Implementation detail:
- I use CRC-32, which is used by Ethernet for this layer. The normal polynomial of CRC-32 is 0x04C11DB7.
- However, there are some differences between my CRC-32 implementation vs Ethernet CRC-32:
    - The initial value of my crc used in calculation is **0**, instead of **0xFFFFFFFF**.
    - The final XOR value of my crc is 0, instead of **0xFFFFFFFF**.
    - My implementation doesn't reflect the input and the result.
- My CRC implementation also use the CRC table technique for optimization.

- Resources for the CRC calculation technique that I use:
    - https://en.wikipedia.org/wiki/Cyclic_redundancy_check
    - http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html#ch6
    - https://barrgroup.com/embedded-systems/how-to/crc-calculation-c-code
- For CRC testing:
    - http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

## Layer 4: Name/Value Pairs
Layer 4 will provide a mechanism for sending and receiving values that have an associated
name.

```c
int l4 write(char *name, int nameLength, char *value, int valueLength)
```
Sends the (name, value) pair to the receiver, where nameLength specifies the number
of bytes in name, and valueLength specifies the number of bytes in value. Returns 1
(true) on success or 0 (false) on error.

```c
int l4 read(char* name, int* nameLength, char* value, int* valueLength)
```
The l4 read function reads a (name, value) pair into the buffers pointed to by name and
value. The nameLength and valueLength parameters are in/out parameters. On input
to the function call, the integers should contain the maximum sizes of their respective
buffers. On successful return from the function call, the integers hold the actual byte
counts received in their respective buffers. The return value of l4 read is 1
(true) on success or 0 (false) on error, including the case where the name buffer or the
value buffer would overflow.