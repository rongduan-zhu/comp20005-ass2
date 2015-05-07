CC=gcc
CFLAGS=-Wall -c
LINKERS=-lm

all: executable

executable: main helper
	$(CC) main.o helper.o -o main.exe $(LINKERS)

main:
	$(CC) $(CFLAGS) main.c

helper:
	$(CC) $(CFLAGS) helper.c

