/// File: queueADT.h
/// Description: Contains functions for queue implementation
/// Author: Nicholas Chieppa nrc4867@rit.edu
///

#include <sys/types.h>

#ifndef QUEUEADT
#define QUEUEADT

typedef struct QUEUE_ST* Queue;

/**
 * queue_create()
 *      create an empty queue
 * returns -
 *      a pointer to an empty queue
 */
Queue queue_create(void);

/**
 * queue_destroy()
 *      frees queue contents
 * args - 
 *      queue - the queue to free
 */
void queue_destroy(Queue queue);

/**
 * queue_clear()
 *      empties a queue of all elements without destroying the queue
 * args -
 *      queue - the queue to empty
 */
void queue_clear(Queue queue);

/**
 * queue_enqueue()
 *      add data to the end of the queue
 * args - 
 *      queue - queue to add on to
 *      data -  data to add
 */
void queue_enqueue(Queue queue, void* data);

/**
 * queue_dequeue()
 *      remove an element from the front of the queue
 * args -
 *      queue - the queue to foward
 * returns - 
 *      the element from the front of the list,
 *      if queue is empty then null
 */
void* queue_dequeue(Queue queue);

/**
 * queue_peak()
 *      get the element from the front of the queue without removing it
 * args - 
 *      queue - queue to look at
 * returns -
 *      the element from the front of the list,
 *      if queue is empty then null
 */
void* queue_peak(Queue queue);

/**
 * queue_empty()
 *      reports if the queue is empty or not
 * args -
 *      queue - queue to inspect
 * returns -
 *      1 if queue empty 0 otherwise
 */ 
int queue_empty(Queue queue);

/**
 * queue_size()
 *      reports the size of the queue
 * args -
 *      queue - queue to inspect
 * returns- 
 *      the size of the queue
 */
size_t queue_size(Queue queue);

#endif
