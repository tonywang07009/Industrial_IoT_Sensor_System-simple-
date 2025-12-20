CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3

# linker need to link all .c file
# Common 
SECURITY_SRC = ..\Security_AES\AES.c


COMMON_SRC = \
   	..\Socket\protocol_display.c \
    ..\Socket\protocol_socket.c \
	..\Parser\protocol_crc_16.c	\
	..\Parser\protocol_parser.c	\

# The So commin
SECURITY_OBJ = $(SECURITY_SRC:.c=.o)
COMMON_OBJ = $(COMMON_SRC:.c=.o)

#The OBJ combine

ALL_COMMON_OBJ = $(SECURITY_OBJ) $(COMMON_OBJ)


# Individual programs
CLIENT_SRC = client_test.c
SERVER_SRC = server_test.c

#Indivual objectfile
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
	

all: client_test.exe server_test.exe

client_test.exe: $(CLIENT_OBJ) $(ALL_COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lws2_32
# The windows need add -lws2_32

server_test.exe: $(SERVER_OBJ) $(ALL_COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lws2_32

clean:
	del /Q *.exe *.o
.PHONY: all clean

