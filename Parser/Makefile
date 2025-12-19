CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O3

SRC = \
    protocol_crc_16.c \
    protocol_parser.c \
    test.c

OBJ = $(SRC:.c=.o)

all: test_parser

test_parser: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) 
    #-lws2_32 -> if you used the windows

clean:
	del /Q test_parser.exe *.o

.PHONY: all clean
