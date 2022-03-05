# Makefile for Project 3
#****************************************************************

COPTIONS = -g -Wall -Werror
LDOPTIONS = 
LIB = 

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: peer

l1.o: l1.c
	gcc $(COPTIONS) -c l1.c

l2.o: l2.c
	gcc $(COPTIONS) -c l2.c

l3.o: l3.c
	gcc $(COPTIONS) -c l3.c

l4.o: l4.c
	gcc $(COPTIONS) -c l4.c

test.o: test.c
	gcc $(COPTIONS) -c test.c

test: l1.o l2.o l3.o l4.o test.o
	gcc $(COPTIONS) -o test l1.o l2.o l3.o l4.o test.o

peer.o: peer.c
	gcc $(COPTIONS) -c peer.c

peer: l1.o l2.o l3.o l4.o peer.o
	gcc $(LDOPTIONS) -o peer l1.o l2.o l3.o l4.o peer.o $(LIB)
	

# delete derived files in preparation for rebuild
clean:
	rm -f *.o peer test
