CC=gcc
CFLAGS=-Wall -c
LINKERS=-lm

all: executable

executable: main helper
	$(CC) main.o helper.o -o main.exe $(LINKERS)

test: tester helper
	$(CC) test.o helper.o -o test.exe $(LINKERS)

tester:
	$(CC) $(CFLAGS) test.c

main:
	$(CC) $(CFLAGS) main.c

helper:
	$(CC) $(CFLAGS) helper.c

