/// file: maze.h 
/// description: contains function definitions for maze creation and solving
/// author: Nicholas R. Chippa
///

#ifndef MAZE
#define MAZE

#define WALL_INPUT "1" /// input character representing a wall
#define PATH_INPUT "0" /// input character representing a path

#define WALL_DISP "#" /// character representing a wall when printed to output
#define PATH_DISP "." /// character representing a path when printed to output

#define BOUND_TOP "-" /// character representing the top/bottom of the maze
#define BOUND_SIDE "|" /// character representing the sides of the maze

typedef struct MAZE_ST {
    int* row; // array of spaces for a row in the maze
    int width; // the width of this row
    struct MAZE_ST *prevRow; // the previous row in the maze
    struct MAZE_ST *nextRow; // the next row in the maze
} Maze;

/**
 * create_maze()
 *      create a maze structure from input
 * args -
 *      input - input stream to get the maze from
 * returns
 *      a maze structure
 */
Maze* create_maze(FILE* input);

/**
 * pretty_print_maze()
 *      prints the maze cleanly
 * args-
 *      maze -  pointer to maze structure to print
 *      output - output stream to print the maze to
 */
void pretty_print_maze(FILE* output, const Maze* maze);


/** 
 * clean_maze()
 *      free all memory allocated by the maze
 * args -
 *      maze - pointer to maze to free
 */
void clean_maze(Maze* maze);

#endif // MAZE
