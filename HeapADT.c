/// File: HeapADT.c
/// Description: Implementation of the heap data type
/// Author: Nicholas Chieppa nrc4687@rit.edu
///

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define PARENT(index) (((index) - 1) / 2) /// index to parent from index
#define LEFT 1 /// offset to left child from index
#define RIGHT 2 /// offset to right child from index
#define CHILD(index, side) (side + (index) * 2) /// index to child from parent

struct Heap_S {
    const void** data; /// pointer to memory used by the heap
    size_t capacity; /// the current max size of the heap
    size_t num; /// the total amount of objects in data
    /// function used for comparing two objects to determine heap ordering
    int (*compFun) (const void * lhs, const void * rhs); 
    /// function used for printing an object when the heaps data is dumped
    void (*dumpEntry) (const void * item, FILE* outfp);
};

#include "HeapADT.h"

/// Implementation from HeapADT.h
/// createHeap()
///     creates a new heap and returns a pointer to the client
Heap createHeap( size_t capacity
               , int (*compFun) (const void * lhs, const void * rhs)
               , void (*dumpEntry) (const void * item, FILE* outfp)) {
    // percondition
    assert(compFun != NULL);
    assert(dumpEntry != NULL);
    
    // allocate space for the heap
    Heap new = (Heap)calloc(capacity, sizeof(struct Heap_S));
    assert(new != NULL);
    // initialize its values
    new->data = malloc(sizeof(void*) * capacity);
    assert(new->data != NULL);
    new->num = 0;
    new->capacity = capacity;
    new->compFun = compFun;
    new->dumpEntry = dumpEntry;

    return new;
}

/// Implementation from HeapADT.h
/// destoryHeap()
///     deallocates memory from a valid heap leaving a null pointer
void destroyHeap(Heap aHeap) {
    assert(aHeap != NULL);
//    assert(aHeap->num == 0); // Client should do thie themsleves
    free(aHeap->data);
    free(aHeap);
    aHeap = NULL;
    
}

/// Implementation from HeapADT.h
/// sizeHeap()
///     reports the size of the heap to the client
size_t sizeHeap(Heap aHeap) {
    return aHeap->num;
}

/// Implementation from HeapADT.h
/// topHeap()
///     reports the entry from the top of the heap to the client
const void * topHeap(const Heap aHeap) {
    assert(aHeap != NULL && aHeap->num != 0);
    const void* top = aHeap->data[0];
    return top;
}

/**
 * smallestIndex()
 *      reports the smallest entry in the heap starting from
 *      an offset, the return value is either the index
 *      provided or one of its two children
 * args - 
 *      aHeap - the heap to look at
 *      index - an offset in aHeap->data to start at
 * returns - 
 *      the index of the smallest item in the heap from index
 *      this function can return either itself or one of its
 *      two children indexes
 */
static size_t smallestIndex(Heap aHeap, size_t index) {
    if(CHILD(index, LEFT) >= aHeap->num)
        return index; // there is no left index
    if(CHILD(index, RIGHT) >= aHeap->num) // there is no right index
        return (!aHeap->compFun(aHeap->data[index], 
                    aHeap->data[CHILD(index, LEFT)]))?CHILD(index, LEFT):index;
    int minIndex = CHILD(index, LEFT);
    if(!aHeap->compFun(aHeap->data[minIndex], aHeap->data[CHILD(index, RIGHT)]))
        minIndex = CHILD(index, RIGHT);
    if(aHeap->compFun(aHeap->data[index], aHeap->data[minIndex]))
        minIndex = index;
    return minIndex;
}

/// Implementation from HeapADT.h
/// removeTopHeap()
///     removes the value at the top of the heap and
///     reports the value removed to the client
void * removeTopHeap(Heap aHeap) {
    assert(aHeap->num != 0);
    void* top = (void *)aHeap->data[0];
    
    aHeap->data[0] = aHeap->data[aHeap->num - 1];
    aHeap->data[--aHeap->num] = NULL;

    size_t index = 0;
    size_t minIndex = smallestIndex(aHeap, index);

    while(minIndex != index) {
        
        const void* temp = aHeap->data[index];
        aHeap->data[index] = aHeap->data[minIndex];
        aHeap->data[minIndex] = temp;

        index = minIndex;
        minIndex = smallestIndex(aHeap, index);
    }

    return top;
}

/// Implementation from HeapADT.h
/// insertHeapItem()
///     insert a new item into the heap, the heap 
///     is then resorted based on the comparison function
///     attached to the heap 
void insertHeapItem(Heap aHeap, const void * item) {
    assert(aHeap != NULL);
    if(aHeap->num == aHeap->capacity) { // ensure heap is not at capacity
        aHeap->data = realloc(aHeap->data, 
                        sizeof(struct Heap_S) * (aHeap->capacity * 2));
        aHeap->capacity *= 2;
//        fprintf(stderr, "The heap is at capacity.\n");
//        assert(aHeap->num != aHeap->capacity);
    }
    aHeap->data[aHeap->num] = item;
    int index = aHeap->num;
    // while comparison between item and parents are true
    while(aHeap->data[index] != aHeap->data[(index - 1)/2] && 
            aHeap->compFun(aHeap->data[index], aHeap->data[(index - 1)/ 2])) {
        const void* temp = aHeap->data[index];
        aHeap->data[index] = aHeap->data[(index - 1) / 2];
        aHeap->data[PARENT(index)] = temp;

        index = PARENT(index); // move the index to parent
    }
    aHeap->num++;
}

/// Implementation from HeapADT.h
/// dumpHeap()
///     prints the order that the heap is storing items using 
///     the heaps dump entry function
void dumpHeap(Heap aHeap, FILE * outfp) {
    for(size_t d = 0; d < aHeap->num; d++) {
        aHeap->dumpEntry(aHeap->data[d], outfp);
    }
}

