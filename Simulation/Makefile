CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3 -I../Data_sturct

# linker need to link all .c file
# Common 
SECURITY_SRC = ../Security_AES/AES.c \
				../Security_AES/tiny-AES-c/aes.c

STATISTICS_SRC = ../Statistics_tool/statistics.c

SIMULATION_SRC = ./Radom/random_fill_parameters.c

COMMON_SRC = \
	./server_function.c \
	./client_function.c \
   	../Socket/protocol_display.c \
    ../Socket/protocol_socket.c \
	../Parser/protocol_crc_16.c	\
	../Parser/protocol_parser.c	

# The So commin the define need check
SECURITY_OBJ = $(SECURITY_SRC:.c=.o)
SIMULATION_OBJ = $(SIMULATION_SRC:.c=.o)
STATISTICS_OBJ = $(STATISTICS_SRC:.c=.o)
COMMON_OBJ = $(COMMON_SRC:.c=.o)

#The OBJ combine

ALL_COMMON_OBJ = $(SECURITY_OBJ) $(COMMON_OBJ) $(STATISTICS_OBJ) $(SIMULATION_OBJ)


# Individual programs
CLIENT_SRC = client_mt.c
SERVER_SRC = server_mt.c

#Indivual objectfile
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
	

all: client_mt server_mt

client_mt: $(CLIENT_OBJ) $(ALL_COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm -lpthread
# The windows need add -lws2_32-
# -lm : is for connect math.h

server_mt: $(SERVER_OBJ) $(ALL_COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -rf *.exe *.o
.PHONY: all clean

