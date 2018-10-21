/// File: maze.c 
/// Description: implementation of maze.h functionality
/// Author: Nicholas Chieppa nrc4867@rit.edu
///

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string.h>

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
        maze->data = (int**) malloc(sizeof(int*));
    } else { // make room for another row in the maze
        maze->data = (int**) realloc(maze->data, 
                                sizeof(int*) * maze->height + 1);
    }
    assert(maze->data != NULL);
    // get the length of data for the line
    int width = strlen(line) - strlen(line) / 2;
    if(maze->width && maze->width != width) {
        fprintf(stderr, "All rows in the maze must have equal widths.");
        exit(EXIT_FAILURE);
    }
    // set data for the current row
    maze->width = width;
    maze->data[maze->height] = malloc(sizeof(int) * width);
    assert(maze->data[maze->height] != NULL);
    for(int linepos = 0, rowpos = 0; rowpos < maze->width;
                 linepos +=2, rowpos++) {
        maze->data[maze->height][rowpos] = strtol(line+linepos, NULL, 10);
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
    
    char* buff;
    size_t len;
    
    while(getline(&buff, &len, input) > 0) {
        add_row(maze, buff);
    }
    
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
            fprintf(output, "%i ", maze->data[row][column]);
        }
        fprintf(output, "%c\n", (row+1 == maze->height)?' ':BOUND_SIDE);
    }
    print_horizontal_bound(output, maze->width);
}
