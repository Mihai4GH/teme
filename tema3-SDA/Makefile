CFLAGS=-Wall -Werror
.PHONY: clean

build: tema3

tema3: main.c hash.o heap.o list.o sgraph.o weightlist.o wgraph.o
	gcc $(CFLAGS) main.c hash.o heap.o list.o sgraph.o weightlist.o wgraph.o -o tema3

hash.o: hash.c hash.h
	gcc $(CFLAGS) -c hash.c

heap.o: heap.c heap.h
	gcc $(CFLAGS) -c heap.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

sgraph.o: sgraph.c sgraph.h
	gcc $(CFLAGS) -c sgraph.c

weightlist.o: weightlist.c weightlist.h
	gcc $(CFLAGS) -c weightlist.c

wgraph.o: wgraph.c wgraph.h
	gcc $(CFLAGS) -c wgraph.c

clean:
	rm -f *.o tema3