#include "queue.h"

Queue initQueue() {
    Queue q = malloc(sizeof(struct queue));
    q->first = q->last = NULL;
    q->size = 0;
    return q;
}

QNode initNode(T data) {
    QNode qnode = malloc(sizeof(struct queue_node));
    qnode->data = data;
    qnode->next = NULL;
    return qnode;
}

Queue enqueue(Queue q, T data) {
    if(q->size == 0) {
        q->first = q->last = initNode(data);
        q->size = 1;
        return q;
    } 
    QNode new_node = initNode(data);
    q->last->next = new_node;
    q->last = new_node;
    q->size++;
    return q;
}

Queue deuqueue(Queue q) {
    QNode tmp;
    if(!isEmptyQueue(q)) {
        tmp = q->first;
        q->first = q->first->next;
        tmp = freeQNode(tmp);
        q->size--;
        if(q->size == 0) 
            q->last = NULL;
    }
    return q;
}

T first(Queue q) {
    if (!isEmptyQueue(q))
        return q->first->data;
    else
        exit(1);
}

int isEmptyQueue(Queue q) {
    if (q == NULL || q->first == NULL || q->size == 0) 
        return 1;
    return 0;
}

QNode freeQNode(QNode node) {
    if (node)
        free(node);
    return NULL;
}

Queue freeQueue(Queue q) {
    while (!isEmptyQueue(q))
        q = deuqueue(q);
    if(q)
        free(q);
    return NULL;
}
