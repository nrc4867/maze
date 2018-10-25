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

struct MAZE_ST {
    int** data;
    int width;
    int height;
};

#include "maze.h"

/**
 * add_row()
 *      add a row to a maze
 * args - 
 *      maze -  the maze to add the row to
 *      line -  a string describing the next
 *              row in the maze
 */
static void add_row(Maze maze, char* line) {  
    if(maze->data == NULL) { // allocate new maze
        maze->data = (int**) malloc(sizeof(int* ));
    } else { // make room for another row in the maze
        maze->data = (int**) realloc(maze->data, 
                                sizeof(int* ) * (maze->height + 1));
    }
    assert(maze->data != NULL);
    // get the length of data for the line
    int width = strlen(line) - 1 - (strlen(line) - 1) / 2;
    if(maze->width && maze->width != width) {
        fprintf(stderr, "All rows in the maze must have equal widths.");
        exit(EXIT_FAILURE);
    }
    // set data for the current row
    maze->width = width;
    maze->data[maze->height] = malloc(sizeof(int *) * width);
    assert(maze->data[maze->height] != NULL);
    for(int linepos = 0, rowpos = 0; rowpos < maze->width;
                 linepos +=2, rowpos++) {
        maze->data[maze->height][rowpos] = (int)strtol(line+linepos, NULL, 10);
    }
    maze->height++;
}

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
    maze->data = NULL;
    
    char* buff = NULL;
    size_t len;
    
    while(getline(&buff, &len, input) > 0) {
        add_row(maze, buff);
    }
    free(buff);

    return maze;
}

void clean_maze(Maze maze) {
    if(maze == NULL) return;
    for(int row = 0; row < maze->height; row++)
        free(maze->data[row]);
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
            fprintf(output, "%i ", maze->data[row][column]);
            #else
            fprintf(output, "%c ", 
                (maze->data[row][column] == 1)?WALL_DISP:
                (maze->data[row][column] > 1)?VALID_PATH:PATH_DISP);
            #endif
        }
        fprintf(output, "%c\n", (row+1 == maze->height)?' ':BOUND_SIDE);
    }
    print_horizontal_bound(output, maze->width);
}

// Acting as backpointer
typedef struct MAZE_TRAVELER_ST {
    int to_visit[2];
    struct MAZE_TRAVELER_ST* prev;
    
} Traveler;

static Traveler* create_traveler(int to_visit[2], Traveler* prev) {
    Traveler* traveler = malloc(sizeof(Traveler));
    traveler->to_visit[0] = to_visit[0];
    traveler->to_visit[1] = to_visit[1];
    traveler->prev = prev;
    return traveler;
}

static void enqueue_neighbors(Queue next, Queue visited, Traveler* curr) {
    queue_enqueue(next, curr);
    queue_enqueue(visited, curr);
}

static void create_neighbors(Queue next, Queue visited, 
                Maze maze, Traveler* curr) {
    // mark this spot as -1 to signifiy that
    // we have traveled to this point already
    maze->data[curr->to_visit[0]][curr->to_visit[1]] = -1;
    // enqueue sorrounding paths
    for(int col = -1; col < 2; col++) {   
        for(int row = -1; row < 2; row++) {
            if(row == col || row == -col) continue;
            int newpoint[] = {curr->to_visit[0] + row,
                                 curr->to_visit[1] + col};
            if(newpoint[0] < maze->height && 
                    newpoint[0] >= 0 &&
                    newpoint[1] < maze->width &&
                    newpoint[1] >= 0 && 
                    maze->data[newpoint[0]][newpoint[1]] == 0)
                enqueue_neighbors(next, visited, 
                    create_traveler(newpoint, curr));
        }
    }

}

int solve_maze(Maze maze) {
    Queue next = queue_create(); // queue of nodes we have to got to
    Queue visited = queue_create(); // queue of nodes we have been to
    // Create the startpoint for the maze
    int startpoint[] = {0,0};
    enqueue_neighbors(next, visited, create_traveler(startpoint, NULL));
    
    // we want to exit this loop if there is nowhere left
    // to travel or we found the exit
    int foundExit = 0;
    Traveler* curr = NULL; 
    while(!queue_empty(next) && !foundExit) {
        curr = (Traveler*)queue_dequeue(next);
        // we only want to look at points that have not been
        // visited or are not walls
        if(!maze->data[curr->to_visit[0]][curr->to_visit[1]]) {
            if(curr->to_visit[0]+1 == maze->height && 
                curr->to_visit[1]+1 == maze->width) {
                foundExit = 1;
            } else {
                create_neighbors(next, visited, maze, curr);
            }   
        }
    }
    
    // if the exit to the maze has been found then 
    // we want to use the backpoints to mark the path
    // we start at depth 2 rather then 1 to differenciate
    // the path from the walls
    for(int depth = 2; foundExit && curr != NULL; depth++) {
        maze->data[curr->to_visit[0]][curr->to_visit[1]] = depth;
        curr = curr->prev;
    }

    while(!queue_empty(visited)) // clear all traversal structs
        free(queue_dequeue(visited));
    queue_destroy(visited);
    queue_destroy(next);

    return maze->data[0][0] - 1; // -1 account for wall
}
