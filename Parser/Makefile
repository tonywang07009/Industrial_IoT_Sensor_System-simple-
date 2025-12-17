CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

test_crc: test.c Parser_crc_16.c
	$(CC) $(CFLAGS) -o $@ $^
clean:
	del /Q test_crc.exe