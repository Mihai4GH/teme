#include "hash.h"

HTable initTable(int initial_capacity) {
    HTable t = (HTable)malloc(sizeof(struct table));
    t->size = 0;
    t->capacity = initial_capacity;
    t->data = (char (*)[NAME_LEN])malloc(t->capacity * NAME_LEN);
    return t;
}

void addToTable(HTable t, char *str) {
    t->size++;
    if(t->size == t->capacity) {
        t->capacity = t->capacity * 2;
        t->data = (char (*)[NAME_LEN])realloc(t->data, t->capacity * NAME_LEN);
    }
    strcpy((t->data)[t->size-1], str);
}

int RequestKey(HTable t, char *str) {
    for (int i = 0; i < t->size; i++) {
        if(!strcmp((t->data)[i], str))
            return i+1;
    }
    return -1;
}

char * RequestId(HTable t, int idx) {
    if(idx > t->size) {
        return NULL;
    }
    return (t->data)[idx];
}

HTable freeTable(HTable t) {
    free(t->data);
    free(t);
    return NULL;
}

void printTableDebugg(HTable t) {
    for(int i = 0; i < t->size; i++) {
        printf("%d -> %s\n", RequestKey(t, (t->data)[i]), RequestId(t, i));
    }
}