CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3

# linker need to link all .c file
SRC = \
   	..\Parser\protocol_crc_16.c \
    ..\Parser\protocol_parser.c \
    protocol_socket.c \
    test.c

OBJ = $(SRC:.c=.o)

all:test_socket
# -lws2_32 -> Windows Winsock library
test_socket: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lws2_32


clean:
	del /Q test_socket.exe *.o

.PHONY: all clean
