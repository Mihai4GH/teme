#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct pair {
    int u, v, nr_sec;
    float *sections, *old;
} Pair; 

// S stands for section
typedef Pair S;

typedef struct list {
    S data;
    struct list *next, *prev;
} *List;

List initList(S data);
List addFirst(List list, S data);
List addLast(List list, S data);
List freeList(List list);


#endif /* __LIST_H__ */