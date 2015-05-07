CC=gcc
CFLAGS=-Wall -ansi -c
LINKERS=-lm

all: main

main:
		$(CC) main.c -o main.exe $(LINKERS)
