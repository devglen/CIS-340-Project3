CC = gcc
CFLAGS = -Wall -g -I.

all: main help showsector showfat traverse structure

main: 
	$(CC) $(CFLAGS) main.c -o main

help: 
	$(CC) $(CFLAGS) help.c -o help

showsector: 
	$(CC) $(CFLAGS) showsector.c -o showsector

showfat: 
	$(CC) $(CFLAGS) showfat.c -o showfat

traverse: 
	$(CC) $(CFLAGS) traverse.c -o traverse

structure: 
	$(CC) $(CFLAGS) structure.c -o structure

clean:
	$(RM) *.o main help showsector showfat traverse structure
