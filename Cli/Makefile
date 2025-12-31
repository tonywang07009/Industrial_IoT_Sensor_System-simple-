CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3 -I../Data_sturct

# linker need to link all .c file
# Common 
SECURITY_SRC = ../Security_AES/AES.c \
				../Security_AES/tiny-AES-c/tiny-AES-c/aes.c

LOG_SRC= ../Log/log.c

STATISTICS_SRC = ../Statistics_tool/statistics.c

SIMULATION_SRC = ../Simulation/Radom/random_fill_parameters.c

COMMON_SRC = \
	../Simulation/server_function.c \
	../Simulation/client_function.c \
   	../Socket/protocol_display.c \
    ../Socket/protocol_socket.c \
	../Parser/protocol_crc_16.c	\
	../Parser/protocol_parser.c	

# The So commin the define need check
SECURITY_OBJ = $(SECURITY_SRC:.c=.o)
LOG_OBJ = $(LOG_SRC:.c=.o)
SIMULATION_OBJ = $(SIMULATION_SRC:.c=.o)
STATISTICS_OBJ = $(STATISTICS_SRC:.c=.o)
COMMON_OBJ = $(COMMON_SRC:.c=.o)

#The OBJ combine

ALL_COMMON_OBJ = $(SECURITY_OBJ) $(COMMON_OBJ) $(STATISTICS_OBJ) $(SIMULATION_OBJ) $(LOG_OBJ)


# Individual programs
CLI_SRC = Cli.interface.c
#Indivual objectfile
Cli_INTEFACE_OBJ = $(CLI_SRC:.c=.o)

TARGET = cli

all: $(TARGET)
$(TARGET): $(Cli_INTEFACE_OBJ) $(ALL_COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -L/usr/local/lib -lm -lpthread -llog
# The target need in fornt
# The windows need add -lws2_32-
# -lm : is for connect math.h
# -L/usr/local/lib -> lD_path specify
clean:
	rm -rf *.exe *.o

.PHONY: all clean

