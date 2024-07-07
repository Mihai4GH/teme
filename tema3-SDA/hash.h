#ifndef __HASH_H__
#define __HASH_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_LEN 30

typedef struct table {
    char (*data)[NAME_LEN];
    int capacity;
    int size;
} *HTable;

HTable initTable(int initial_capacity);
void addToTable(HTable t, char *str);
int RequestKey(HTable t, char *str);

// Should be called for idx = true_idx - 1
char* RequestId(HTable t, int idx);
HTable freeTable(HTable t);
void printTableDebugg(HTable t);


#endif /* hash.h */