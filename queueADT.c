/// File: queueADT.c 
/// Description: Contains implementation for queueADT.h
/// Author: Nicholas Chieppa nrc4867@rit.edu
///

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>

#define QUEUE_ALLOC_UNIT 5

/// A hidden linked list that contains queue elements
typedef struct QUEUE_LINKED_LIST {
    void* value; /// any value
    struct QUEUE_LINKED_LIST* next; /// link to next element in the queue
} Qlinked;

struct QUEUE_ST{
    Qlinked* front; /// the front of the queue
    Qlinked* back; /// the back of the queue
    size_t num; /// the amount of elements in the queue
};

#include "queueADT.h"

/**
 * queue_create()
 *      create an empty queue
 * returns - 
 *      a pointer to an empty queue
 */
Queue queue_create(void) {
    Queue queue = (Queue)malloc(sizeof(struct QUEUE_ST));
    
    queue->front = queue->back = NULL;
    queue->num = 0;
    
    return queue;
}

/**
 * queue_destroy()
 *      frees queue contents
 * args - 
 *      queue - the queue to free
 */
void queue_destroy(Queue queue) {
    assert(queue != NULL);
    queue_clear(queue);
    free(queue);
}

/**
 * queue_clear()
 *      empties a queue of all elements without destroying the queue
 * args -
 *      queue - the queue to empty
 */
void queue_clear(Queue queue) {
    while(queue->front != NULL) {
        Qlinked* temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    queue->back = NULL;
    queue->num = 0;
}

/**
 * queue_enqueue()
 *      add data to the end of the queue
 * args - 
 *      queue - queue to add on to
 *      data -  data to add
 */
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

/**
 * queue_dequeue()
 *      remove an element from the front of the queue
 * args -
 *      queue - the queue to foward
 * returns - 
 *      the element from the front of the list,
 *      if queue is empty then null
 */
void* queue_dequeue(Queue queue) {
    if(queue_empty(queue)) return NULL;

    void* value = queue->front->value;

    Qlinked* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    
    queue->num--; 
    return value;
}

/**
 * queue_peak()
 *      get the element from the front of the queue without removing it
 * args - 
 *      queue - queue to look at
 * returns -
 *      the element from the front of the list,
 *      if queue is empty then null
 */
void* queue_peak(Queue queue) {
    if(queue_empty(queue)) return NULL;
    return queue->front->value;
}

/**
 * queue_size()
 *      reports the size of the queue
 * args -
 *      queue - queue to inspect
 * returns- 
 *      the size of the queue
 */
size_t queue_size(Queue queue) {
    return queue->num;
}

/**
 * queue_empty()
 *      reports if the queue is empty or not
 * args -
 *      queue - queue to inspect
 * returns -
 *      1 if queue empty 0 otherwise
 */ 
int queue_empty(Queue queue) {
    return queue->num == 0;
}
