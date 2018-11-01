/// File: maze.c 
/// Description: implementation of maze.h functionality
/// Author: Nicholas Chieppa nrc4867@rit.edu
///

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "queueADT.h"
#include "stackADT.h"
#include "HeapADT.h"


struct MAZE_ST {
    int* data;
    int width;
    int height;
};

#include "maze.h"

#define COORDS(y, x) (y*maze->width+x)
#define ABOVE(index) (index - maze->width)
#define BELOW(index) ((index + maze->width < maze->width*maze->height) \
                            ?index+maze->width:-1)
#define LEFT(index) ((index%maze->width)?index-1:-1)
#define RIGHT(index) ((index%maze->width != maze->width - 1)?index+1:-1)

/**
 * create_maze()
 *      create a maze structure from input
 * args - 
 *      input - input stream to get the maze from
 * returns - 
 *      a pointer to a maze structure
 */
Maze create_maze(FILE* input) {
    Maze maze = (Maze)malloc(sizeof(struct MAZE_ST));
    assert(maze != NULL);

    maze->width = maze->height = 0;
    
    fseek(input, 0, SEEK_END);
    long size = ftell(input);
    fseek(input, 0, SEEK_SET);
    char* wholeMaze = malloc(size + 1);
    fread(wholeMaze, size, 1, input);
    
    maze->width = (int)strspn(wholeMaze, " 10");
    if(maze->width % 2) maze->width++;
    maze->height = size / maze->width;
    maze->width /= 2;    
    
    maze->data = malloc(sizeof(int) * (maze->width * maze->height));
    for(int i = 0, sp = 0; i < maze->width * maze->height; i++, sp += 2)
        maze->data[i] = (int)strtol(wholeMaze + sp, NULL, 10);
    free(wholeMaze);
    #ifdef DEBUG
    fprintf(stdout,  "%ld\n", size);
    printf("w%i h%i\n", maze->width, maze->height);
    #endif
    return maze;
}


/** 
 * clean_maze()
 *      free all memory allocated by the maze
 * args -
 *      maze - pointer to maze to free
 */
void clean_maze(Maze maze) {
    if(maze == NULL) return;
    free(maze->data);
    free(maze);
}

/**
 * print_horizontal_bound()
 *      used for printing either the vertical or horizontal bound on the maze
 * args -
 *      output - output stream to print the maze to
 *      length - the length of the bound
 */
static void print_horizontal_bound(FILE* output, int length) {
    fprintf(output, "%c", BOUND_SIDE); // Leftmost bound char
    while(length-- > 0)
    // need two bounds for each position in the maze
        fprintf(output, "%c%c", BOUND_TOP, BOUND_TOP);
    // print the bound character following the last space in the
    // maze followed by the rightmost bound char
    fprintf(output, "%c%c\n", BOUND_TOP, BOUND_SIDE);
    
}

/**
 * pretty_print_maze()
 *      prints the maze cleanly
 * args-
 *      maze -  pointer to maze structure to print
 *      output - output stream to print the maze to
 */
void pretty_print_maze(const Maze maze, FILE* output) {
    print_horizontal_bound(output, maze->width);
    for(int row = 0; row < maze->height; row++) {
        fprintf(output, "%c ", (!row)?' ':BOUND_SIDE);
        for(int column = 0; column < maze->width; column++) {
            #ifdef DEBUG
            fprintf(output, "%i ", maze->data[row*maze->width + column]);
            #else
            fprintf(output, "%c ", 
                (maze->data[row*maze->width + column] == 1)?WALL_DISP:
                (maze->data[row*maze->width + column] > 1)?VALID_PATH:PATH_DISP);
            #endif
        }
        fprintf(output, "%c\n", (row+1 == maze->height)?' ':BOUND_SIDE);
    }
    print_horizontal_bound(output, maze->width);
}

static double distance(int x1, int y1, int x2, int y2) {
    return sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

/// represent traversing through the maze
typedef struct MAZE_TRAVELER_ST {
    int to_visit; // the position on the map this traveler is assoicated with
    float distance; // distance from to_vist to start
    struct MAZE_TRAVELER_ST* prev; // the traveler that spawned this one
} Traveler;

void dumpTraveler(const void* item, FILE* outfp) {
    fprintf(stdout, "dump\n");
}

int cmpTravelers(const void * lhs, const void* rhs) {
    Traveler* t1 = (Traveler*)lhs;
    Traveler* t2 = (Traveler*)rhs;
    return t1->distance >= t2->distance;
}

/**
 * create_traveler()
 *      create a pointer to a traveler object 
 * args -
 *      to_visit - the space this traveler is associated with
 *      distance - the distance from this poistion to the start
 *      prev     - the traveler that spawned this one
 * returns -
 *      a pointer to a traveler 
 */
static Traveler* create_traveler(int to_visit, double distance, Traveler* prev) {
    Traveler* traveler = malloc(sizeof(Traveler));
    traveler->to_visit = to_visit;
    traveler->distance = distance;
    traveler->prev = prev;
    return traveler;
}

/**
 * create_neighbors()
 *      find an create travelers to visit valid spaces 
 *      adjectent to the space on the maze that we are looking at.
 *      A valid space is one that is in bounds and 
 *      currently marked with a 0.
 *      After the space is traveled to it is marked with the indicator -1
 * args - 
 *      next - a pointer to a queue that will decide what space we will travel
 *             to next and iterate over
 *      maze - a pointer to a maze to look at
 *      curr - the current space that we have traveled to
 */
static void create_neighbors(Heap next,
                Maze maze, Traveler* curr) {
    // mark this spot as -1 to signifiy that
    // we have traveled to this point already
    maze->data[curr->to_visit] = -1;
    // enqueue sorrounding paths
    int index = curr->to_visit;
    if(ABOVE(index) >= 0 && !maze->data[ABOVE(index)])
        insertHeapItem(next, 
            create_traveler(ABOVE(index), distance(ABOVE(index)%maze->width, (int)(ABOVE(index)/maze->width), maze->width, maze->height), curr));
    if(BELOW(index) >= 0 && !maze->data[BELOW(index)])
        insertHeapItem(next, 
            create_traveler(BELOW(index), distance(BELOW(index)%maze->width, (int)(BELOW(index)/maze->width), maze->width, maze->height), curr));
    if(LEFT(index) >= 0 && !maze->data[LEFT(index)])
        insertHeapItem(next, 
            create_traveler(LEFT(index), distance(LEFT(index)%maze->width, (int)(LEFT(index)/maze->width), maze->width, maze->height), curr));
    if(RIGHT(index) >= 0 && !maze->data[RIGHT(index)])
        insertHeapItem(next, 
            create_traveler(RIGHT(index), distance(RIGHT(index)%maze->width, (int)(RIGHT(index)/maze->width), maze->width, maze->height), curr));

}

/**
 * solve_maze()
 *      modifies the maze rows such that the 
 *      shortest path is marked with the steps to solve the maze.
 *
 *      the maze is modified such that future calls
 *      to pretty_print_maze() will print VALID_PATH
 *      on the path towards the exit
 * args - 
 *      maze - the maze to solve
 * returns -
 *      the shorest path distance (min: 1),
 *      or -1 if there is no solution
 */
int solve_maze(Maze maze) {
    Heap next = createHeap(10, cmpTravelers, dumpTraveler); // queue of nodes we have to got to
    StackADT visited = stk_create();
    // Create the startpoint for the maze
    insertHeapItem(next, create_traveler(0, distance(0,0,maze->width, maze->height), NULL));
    
    // we want to exit this loop if there is nowhere left
    // to travel or we found the exit
    int foundExit = 0;
    Traveler* curr = NULL; 
    while(sizeHeap(next) && !foundExit) {
        curr = (Traveler*)removeTopHeap(next);
        // we only want to look at points that have not been
        // visited or are not walls
        if(!maze->data[curr->to_visit]) {
            if(curr->to_visit == maze->height*maze->width - 1) {
                maze->data[maze->width*maze->height - 1]  = curr->distance;
                foundExit = 1;
            } else {
                create_neighbors(next, maze, curr);
            }   
        }
        //free(curr);
        stk_push(visited, curr);
    }
    
    if(foundExit) {
    int distance = 2;
        do {
            maze->data[curr->to_visit] = distance++;
            curr = curr->prev;
        } while(curr != NULL);
    }
    
   while(!stk_empty(visited))
       free(stk_pop(visited));
    
    while(sizeHeap(next)) // clear all traversal structs
        free(removeTopHeap(next));
    destroyHeap(next);
    stk_destroy(visited);

    return (foundExit)?maze->data[0] - 1:-1;  
}

