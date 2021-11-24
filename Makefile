CC=g++
CFLAGS=-I.

all: main.o map.o

main.o: main.cpp file.h
	$(CC) main.cpp -g -o main.o

map.o: map.cpp file.h
	$(CC) map.cpp -g -o map.o

.PHONY: clean
clean:
	rm -f *.o