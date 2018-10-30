/// File: maze.c 
/// Description: implementation of maze.h functionality
/// Author: Nicholas Chieppa nrc4867@rit.edu
///

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queueADT.h"
#include "stackADT.h"

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
 *
 *
 *
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

void clean_maze(Maze maze) {
    if(maze == NULL) return;
    free(maze->data);
    free(maze);
}

void print_horizontal_bound(FILE* output, int length) {
    fprintf(output, "%c", BOUND_SIDE);
    while(length-- > 0)
        fprintf(output, "%c%c", BOUND_TOP, BOUND_TOP);
    fprintf(output, "%c%c\n", BOUND_TOP, BOUND_SIDE);
    
}

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

// Acting as backpointer
typedef struct MAZE_TRAVELER_ST {
    int to_visit;
    int distance;
    struct MAZE_TRAVELER_ST* prev;
} Traveler;

static Traveler* create_traveler(int to_visit, int distance, Traveler* prev) {
    Traveler* traveler = malloc(sizeof(Traveler));
    traveler->to_visit = to_visit;
    traveler->distance = distance;
    traveler->prev = prev;
    return traveler;
}

static void create_neighbors(Queue next,
                Maze maze, Traveler* curr) {
    // mark this spot as -1 to signifiy that
    // we have traveled to this point already
    maze->data[curr->to_visit] = -1;
    // enqueue sorrounding paths
    int index = curr->to_visit;
    if(ABOVE(index) >= 0 && !maze->data[ABOVE(index)])
        queue_enqueue(next, 
            create_traveler(ABOVE(index), curr->distance + 1, curr));
    if(BELOW(index) >= 0 && !maze->data[BELOW(index)])
        queue_enqueue(next, 
            create_traveler(BELOW(index), curr->distance + 1, curr));
    if(LEFT(index) >= 0 && !maze->data[LEFT(index)])
        queue_enqueue(next, 
            create_traveler(LEFT(index), curr->distance + 1, curr));
    if(RIGHT(index) >= 0 && !maze->data[RIGHT(index)])
        queue_enqueue(next, 
            create_traveler(RIGHT(index), curr->distance + 1, curr));

}

int solve_maze(Maze maze) {
    Queue next = queue_create(); // queue of nodes we have to got to
    StackADT visited = stk_create();
    // Create the startpoint for the maze
    queue_enqueue(next, create_traveler(0, 2, NULL));
    
    // we want to exit this loop if there is nowhere left
    // to travel or we found the exit
    int foundExit = 0;
    Traveler* curr = NULL; 
    while(!queue_empty(next) && !foundExit) {
        curr = (Traveler*)queue_dequeue(next);
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
        do {
            maze->data[curr->to_visit] = curr->distance;
            curr = curr->prev;
        } while(curr != NULL);
    }
    
   while(!stk_empty(visited))
       free(stk_pop(visited));
    
    while(!queue_empty(next)) // clear all traversal structs
        free(queue_dequeue(next));
    queue_destroy(next);
    stk_destroy(visited);

    return maze->data[maze->width*maze->height - 1] - 1;  
}
