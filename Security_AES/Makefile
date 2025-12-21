CC     = gcc
CFLAGS = -O3 -Wall

SRCS = test_secu.c \
       AES.c \
       tiny-AES-c/aes.c

OBJS = $(SRCS:.c=.o)

test_secu: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o test_secu
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	del /Q test_secu.exe *.o

.PHONY: test_secu clean
