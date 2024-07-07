CFLAGS=-Wall -Werror
.PHONY: clean

build: tema1

tema1: main.c train.o queue.o
	gcc main.c train.o queue.o -g $(CFLAGS) -o tema1

train.o: train.c train.h
	gcc -c train.c -g

queue.o: queue.c queue.h
	gcc -c queue.c -g

run: build
	./tema1

clean:
	rm -f *.o tema1