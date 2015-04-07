CC = gcc
CFLAGS = -Wall -g -c

all: main help showfat showsector structure traverse

main: main.o
	$(CC) main.o -o main

main.o: main.c
	$(CC) $(CFLAGS) main.c

help: help.o
	$(CC) help.o -o help

help.o: help.c
	$(CC) $(CFLAGS) help.c

showfat: showfat.o
	$(CC) showfat.o -o showfat

showfat.o: showfat.c
	$(CC) $(CFLAGS) showfat.c

showsector: showsector.o
	$(CC) showsector.o -o showsector

showsector.o: showsector.c
	$(CC) $(CFLAGS) showsector.c

structure: structure.o
	$(CC) structure.o -o structure

structure.o: structure.c
	$(CC) $(CFLAGS) structure.c

traverse: traverse.o
	$(CC) traverse.o -o traverse

traverse.o: traverse.c
	$(CC) $(CFLAGS) traverse.c

clean:
	$(RM) *.o $(EXECS)
