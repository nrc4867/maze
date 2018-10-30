/// file: maze.h 
/// description: contains function definitions for maze creation and solving
/// author: Nicholas R. Chippa
///

#ifndef MAZE
#define MAZE

#define WALL_DISP '#' /// character representing a wall when printed to output
#define PATH_DISP '.' /// character representing a path when printed to output
#define VALID_PATH '+' /// character representing the found path

#define BOUND_TOP '-' /// character representing the top/bottom of the maze
#define BOUND_SIDE '|' /// character representing the sides of the maze

/// represent a maze
typedef struct MAZE_ST* Maze;

/**
 * create_maze()
 *      create a maze structure from input
 * args -
 *      input - input stream to get the maze from
 * returns
 *      a pointer to a maze structure
 */
Maze create_maze(FILE* input);

/**
 * pretty_print_maze()
 *      prints the maze cleanly
 * args-
 *      maze -  pointer to maze structure to print
 *      output - output stream to print the maze to
 */
void pretty_print_maze(const Maze maze, FILE* output);

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
int solve_maze(Maze maze);


/** 
 * clean_maze()
 *      free all memory allocated by the maze
 * args -
 *      maze - pointer to maze to free
 */
void clean_maze(Maze maze);

#endif // MAZE
