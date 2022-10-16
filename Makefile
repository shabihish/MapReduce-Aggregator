CC=g++
CFLAGS=-I.

all: main.o map.o reduce.o

main.o: main.cpp file.h
	$(CC) main.cpp -g -o main.o

map.o: map.cpp file.h
	$(CC) map.cpp -g -o map.o

reduce.o: reduce.cpp file.h
	$(CC) reduce.cpp -g -o reduce.o

.PHONY: clean
clean:
	rm -f *.o