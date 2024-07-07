CFLAGS=-Wall -Werror -g

.PHONY: build clean

build: tema2

tema2: main.o tree.o queue.o
	gcc $(CFLAGS) main.o tree.o queue.o -o tema2

main.o: main.c
	gcc $(CFLAGS) -c -o $@ $<

tree.o: tree.c tree.h
	gcc $(CFLAGS) -c -o $@ $<

queue.o: queue.c queue.h
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o tema2 tree.png
