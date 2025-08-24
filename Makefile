CC = gcc
CFLAGS = -O0 -Wall
output: main
	./main
build: build.o
	$(CC) $(CFLAGS) main.o -o main
build.o: build.c
	$(CC) $(CFLAGS) -c main.c -o main.o
clean:
	rm -f main.o main
