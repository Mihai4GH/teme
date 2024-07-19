#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"


CQueue init_queue() {
    // Function that initialize the queue
    CQueue q = (CQueue)malloc(sizeof(struct Queue));
    q->first = q->last = NULL;
    q->QSwitch = 1;
    return q;
}

void enqueue(CQueue q, const char *str, FILE *input) {
    CNode aux = (CNode)malloc(sizeof(struct commandNode));
    aux->ArgStr = NULL;
    aux->next = aux->prev = NULL;
    strcpy(aux->command, str);
    if(strcmp(str, "MOVE_LEFT") == 0 || strcmp(str, "MOVE_RIGHT") == 0 ||
        strcmp(str, "CLEAR_CELL") == 0 || strcmp(str, "CLEAR_ALL") == 0) {
            aux->ArgStr = NULL;
    } else if(strcmp(str, "WRITE") == 0 || strcmp(str, "INSERT_LEFT") == 0 ||
        strcmp(str, "INSERT_RIGHT") == 0) {
            aux->ArgStr = (char *)malloc(sizeof(char) * ARG_SIZE);
            fscanf(input, "%s", aux->ArgStr);
    } else {
        aux->ArgStr = (char *)malloc(sizeof(char) * ARG_SIZE);
        fscanf(input, "%s", aux->ArgStr);
    }
    // CNode aux will contain the command to be procces
    // and, eventually, it's argument if needed
    // The commands are enqueued either at the end of the queue,
    // or the beginning. If QSwitch is 1, operations will pe performed
    // at the beginning of the queue, otherwise to the end.
    if(q->QSwitch == 1){        
        if(q->first == NULL && q->last == NULL) {
            q->first = q->last = aux;
            return;
        }
        aux->prev = q->last;
        q->last->next = aux;
        q->last = aux;
    } else {
        if(q->first == NULL && q->last == NULL) {
            q->first = q->last = aux;
            return;
        }
        aux->next = q->first;
        q->first->prev = aux;
        q->first = aux;
    }
}

void dequeue_first(CQueue q) {
    /* Function that will pop the firs element of
    the queue. Used when QSwitch is set to 1 */
    if(q->first == NULL){
        return;
    }
    if(q->first->ArgStr != NULL) {
        free(q->first->ArgStr);
    }
    if(q->first == q->last) {
        free(q->first);
        q->first = q->last = NULL;
        return;
    } 
    CNode aux = q->first;
    q->first = aux->next;
    q->first->prev = NULL;
    free(aux);
}

void dequeue_last(CQueue q) {
    /* Function that will pop the last element of
    the queue. Used when QSwitch is set to -1 */
    if(q->last == NULL) {
        return;
    }
    if(q->last->ArgStr != NULL) {
        free(q->last->ArgStr);
    }
    if(q->first == q->last) {
        free(q->last);
        q->first = q->last = NULL;
        return;
    }
    CNode aux = q->last;
    q->last = aux->prev;
    q->last->next = NULL;
    free(aux);
}

void destroy_queue(CQueue q) {
    /* Destroying the queue and freeing the memory */
    while(q->last != NULL) {
        dequeue_last(q);
    }
    free(q);
}
