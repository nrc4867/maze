/// File: queueADT.c 
/// Description: Contains implementation for queueADT.h
/// Author: Nicholas Chieppa nrc4867@rit.edu
///

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>

#define QUEUE_ALLOC_UNIT 5

typedef struct QUEUE_LINKED_LIST {
    void* value;
    struct QUEUE_LINKED_LIST* next;
} Qlinked;

struct QUEUE_ST{
    Qlinked* front; /// the front of the queue
    Qlinked* back; /// the back of the queue
    size_t num; /// the amount of elements in the queue
};

#include "queueADT.h"

Queue queue_create(void) {
    Queue queue = (Queue)malloc(sizeof(struct QUEUE_ST));
    
    queue->front = queue->back = NULL;
    queue->num = 0;
    
    return queue;
}

static void destroy_contents(Queue queue) { 
    while(queue->front != NULL) {
        Qlinked* temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    queue->back = NULL;
}

void queue_destroy(Queue queue) {
    assert(queue != NULL);
    destroy_contents(queue);
    free(queue);
}

void queue_clear(Queue queue) {
    destroy_contents(queue);
    queue->num = 0;
}

void queue_enqueue(Queue queue, void* data) {
   if(queue->front == NULL) { // Queue is empty
        queue->front = queue->back = malloc(sizeof(Qlinked));
        assert(queue->front != NULL);
    } else {
        queue->back->next = malloc(sizeof(Qlinked));
        assert(queue->back->next != NULL);
        queue->back = queue->back->next;
        queue->back->next = NULL;
    } 
    
    queue->back->value = data;
    queue->num += 1;
}

void* queue_dequeue(Queue queue) {
    if(queue_empty(queue)) return NULL;

    void* value = queue->front->value;

    Qlinked* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    
    queue->num--; 
    return value;
}

void* queue_peak(Queue queue) {
    if(queue_empty(queue)) return NULL;
    return queue->front->value;
}

size_t queue_size(Queue queue) {
    return queue->num;
}

int queue_empty(Queue queue) {
    return queue->num == 0;
}
