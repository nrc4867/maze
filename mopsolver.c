/// file: mopsolver.c
/// description: solves mazes
/// author: Nicholas R. Chieppa
///

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define WALL_INPUT "1" /// input character representing a wall
#define PATH_INPUT "0" /// input character representing a path

#define WALL_DISP "#" /// character representing a wall when printed to output
#define PATH_DISP "." /// character representing a path when printed to output

#define BOUND_TOP "-" /// character representing the top/bottom of the map
#define BOUND_SIDE "|" /// character representing the sides of the map

#define VALID_FLAGS "hdspi:o:" /// flags respected by this program
#define ARG_COUNT 6 /// amount of flags respected by this program

#define PRETTY_PRINT 0 /// default value for -d
#define PRINT_STEP_COUNT 0 /// default value for -s
#define PRINT_OPTIMAL 0 /// default value for -p
#define INPUT_STREAM stdin /// default value for -i
#define OUTPUT_STREAM stdout /// default value for -o

#define STEPS_OUTPUT "Solution in %i steps" //

/**
 * usage_message()
 *      prints the usage message for the program
 */
void usage_message() {
    printf("USAGE:\nmopsolver [-hdsp] [-i INFILE] [-o OUTFILE]");
}

/**
 * help_message()
 *      prints the help message for flag -h
 */
void help_message() {
    char *messages[ARG_COUNT] = {
        "Print this helpful message to stdout and exit.", // -h
        "Pretty print (display) the maze after reading.", // -d
        "Print shortest solution steps.",                 // -s
        "Read maze from INFILE.",                         // -i
        "Print an optimal path.",                         // -p
        "Write all output to OUTFILE."                    // -o
    };

    char *defaults[ARG_COUNT] = {
        "",             // -h
        "off",          // -d
        "off",          // -s
        "off",          // -p
        "stdio",        // -i
        "stdout"        // -o
    };
    
    printf("Options:\n");
}

/**
 * main()
 *      runs the maze solver application
 * args - 
 *      argc -   the amount of arguments passed into this program
 *      argv -   the arguments passed into this program as a string array
 */
int main(int argc, char** argv) {
    //Initialize program flags
    int d = PRETTY_PRINT, s = PRINT_STEP_COUNT, p = PRINT_OPTIMAL;
    FILE *i = INPUT_STREAM;
    FILE *o = OUTPUT_STREAM;
}

