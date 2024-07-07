#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>

typedef struct node *T;

typedef struct queue_node {
    T data;
    struct queue_node *next;
} *QNode;

typedef struct queue {
    QNode first, last;
    int size;
} *Queue;

Queue initQueue();
QNode initNode(T data);
Queue enqueue(Queue q, T data);
Queue deuqueue(Queue q);
T first(Queue q);
int isEmptyQueue(Queue q);
QNode freeQNode(QNode node);
Queue freeQueue(Queue q);


#endif