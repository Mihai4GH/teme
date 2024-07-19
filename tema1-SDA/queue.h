#ifndef QUEUE_GUARD
#define QUEUE_GUARD

#include <stdlib.h>

#define ARG_SIZE 20

typedef struct commandNode{
    char command[20];
    char *ArgStr;
    struct commandNode *next;
    struct commandNode *prev;
} *CNode;               // CNode = Comamand Node

typedef struct Queue{
    CNode first, last;  // QSwitch determines the order of operations performed on the queue
    int QSwitch;        // 1 -> direct way, -1 -> from end of the queue to the start
} *CQueue;              // CQueue = Command Queue

// Queue function

CQueue init_queue();
void enqueue(CQueue q, const char *str, FILE *input);
void dequeue_first(CQueue q);
void dequeue_last(CQueue q);
void destroy_queue(CQueue q);

#endif // QUEUE_GUARD