/// file: maze.c
/// description: implementation of maze.h 
/// author: Nicholas R. Chieppa nrc4867
///

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct MAZE_ST {
    int* row;
    int width;
    struct MAZE_ST* prevRow;
    struct MAZE_ST* nextRow;
};

#include "maze.h"

/**
 * create_row()
 *      create a maze row for 
 * args -
 *      line - a line describing a row in a maze
 * returns - 
 *      a pointer to a maze
 */
static Maze create_row(char* line) {
    Maze maze = malloc(sizeof(struct MAZE_ST));
    assert(maze != NULL);
    maze->width = strlen(line) - strlen(line)/2; // line without spaces
    maze->row = malloc(sizeof(int) * maze->width);
    maze->nextRow = NULL;
    int lp = 0; // line position
    int mp = 0; // maze position
    for(; mp < maze->width; lp += 2, mp++) {
        *(maze->row+mp) = strtol(line+lp, NULL, 10); 
    }
    return maze;
}

/**
 * create_maze()
 *      create a maze out of a 2d array
 * args -
 *      input - input stream to get the maze from
 * returns
 *      a pointer to a maze, if no rows then null pointer
 */
Maze create_maze(FILE* input) {
    Maze maze = NULL; // the top of the maze
    Maze curr = NULL; // the row being worked with
    Maze prev = NULL; // the previous row made

    char* buff = NULL;
    size_t len;
    while(getline(&buff, &len, input) > 0) {
        if(curr == NULL) { // Create first row of maze
            maze = curr = create_row(buff);
        }
        else { // create successive rows
            curr->nextRow = create_row(buff);
            curr = curr->nextRow;
        }
        curr->prevRow = prev; // move the current and previous row up
        prev = curr; 
    }
    free(buff);
    return maze;
}

/**
 * print_vertical_bound()
 *      prints the bound for the top and bottom of the maze
 * args -
 *      output - output stream to print bar onto
 *      width  - width of the line to print
 */
static void print_vertical_bound(FILE* output, int width) {
    fprintf(output, "%s", BOUND_SIDE);
    for(int i = 0; i < width * 2; i++) 
        fprintf(output, "%s", BOUND_TOP);
    fprintf(output, "%s%s\n", BOUND_TOP, BOUND_SIDE); 
}

/**
 * pretty_print_maze()
 *      prints the maze cleanly
 * args-
 *      maze   - a pointer to an array of intgers represting a maze
 *      output - output stream to print the maze to
 */
void pretty_print_maze(FILE* output, const Maze maze) {
    if(maze == NULL) return;
    if(maze->prevRow == NULL) 
        print_vertical_bound(output, maze->width);
    fprintf(output, "%s ", (maze->prevRow == NULL)?" ":BOUND_SIDE);
    for(int pos = 0; pos < maze->width; pos++) {
        if(*(maze->row+pos) == 1)
            fprintf(output, "%s ", WALL_DISP);
        else if(*(maze->row+pos) > 1)
            fprintf(output, "%s ", VALID_PATH);
        else
            fprintf(output, "%s ", PATH_DISP);
    }
    fprintf(output, "%s\n", (maze->nextRow == NULL)?" ":BOUND_SIDE);
    if(maze->nextRow == NULL)
        print_vertical_bound(output, maze->width);
    pretty_print_maze(output, maze->nextRow);
}

static int find_path(Maze maze, int offset) {
    if(*(maze->row+offset) != 0) return 0; // in a wall
    if(maze->nextRow == NULL && offset == maze->width - 1) { // found the end
        // start counting the backtrace
        // counting starts at 2 because 1 is reservered for walls
        return *(maze->row+offset) = 2;
    }
    int path = 0; // distance from the last square to the exit
    *(maze->row+offset) = -1; // block the searcg from going backwards
    if(offset + 1 != maze->width) { // move right
        path = find_path(maze, offset + 1);
    } 
    if(!path && offset - 1 >= 0) { // move left
        path = find_path(maze, offset -1);
    }
    if(!path && maze->prevRow != NULL) { // move up
        path = find_path(maze->prevRow, offset);
    }
    if(!path && maze->nextRow != NULL) { // move down
        path = find_path(maze->nextRow, offset);
    }
    return (path > 0)?*(maze->row+offset) = path + 1:0;
}

int solve_maze(Maze maze) {
    // minus 1 from the result of find path
    // to account for the fact that a wall 
    // is represented as a 1
    return find_path(maze, 0) - 1;
}

/** 
 * clean_maze()
 *      free all memory allocated by the maze
 * args -
 *      maze - pointer to maze to free
 */
void clean_maze(Maze maze) {
    if(maze == NULL) return;
    free(maze->row); // free the values
    if(maze->prevRow != NULL) free(maze->prevRow); // free previous row
    (maze->nextRow == NULL)?free(maze):clean_maze(maze->nextRow); // move up
}
