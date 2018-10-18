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

/**
 * create_maze()
 *      create a maze out of a 2d array
 * args -
 *      input - input stream to get the maze from
 * returns
 *      a pointer to an array of intgers representing a maze
 */
int** create_maze(FILE* input);

/**
 * pretty_print_maze()
 *      prints the maze cleanly
 * args-
 *      maze -  a pointer to an array of intgers represting a maze
 *      output - output stream to print the maze to
 */
void pretty_print_maze(FILE* output, int** maze);

#endif // MAZE
