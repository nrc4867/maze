/// file: mopsolver.c
/// description: solves mazes
/// author: Nicholas R. Chieppa
///

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

#define VALID_FLAGS "hdspi:o:" /// flags respected by this program
#define ARG_COUNT 6 /// amount of flags respected by this program

#define PRETTY_PRINT 0 /// default value for -d
#define PRINT_STEP_COUNT 0 /// default value for -s
#define PRINT_OPTIMAL 0 /// default value for -p
#define INPUT_STREAM stdin /// default value for -i
#define OUTPUT_STREAM stdout /// default value for -o

/**
 * protected_free()
 *      checks that a pointer is allocated prior to freeing
 * args -
 *      pointer - location to a memory address to free
 */
void protected_free(void* pointer) {
    if(pointer != NULL)
        free(pointer);
}

/**
 * usage_message()
 *      prints the usage message for the program
 * args -
 *      stream - location to print the usage information
 */
void usage_message(FILE* stream) {
    fprintf(stream, "USAGE:\nmopsolver [-hdsp] [-i INFILE] [-o OUTFILE]\n");
}

/**
 * help_message()
 *      prints the help message for flag -h
 */
void help_message() { 
    printf("Options:\n");
    printf("\t-h\tPrint this helpful message to stdout and exit.\n");
    printf("\t-d\tPretty print (display) the maze "
                "after reading.\t(Default: off)\n");
    printf("\t-s\tPrint shorest solution steps."
                "\t\t\t(Default: off)\n");
    printf("\t-i INFILE\tRead maze from INFILE."
                "\t\t\t(Default: stdin)\n");
    printf("\t-o OUTFILE\tWrite all output to OUTFILE."
                "\t\t(Default: stdout)\n");
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
    
    // input and output locations should only be changed after
    // all arguments are read, protects against multiple uses of -i/-o
    char *inputloc = NULL;
    char *outputloc = NULL; 
    
    int exit = EXIT_SUCCESS; // exit code

    char c;
    extern char* optarg;
    while((c = getopt(argc, argv, VALID_FLAGS)) != -1) {
        switch(c) {
            case 'h':
                usage_message(stdout);
                help_message();
                goto end_program; // free all allocated memory before exiting 
                break;
            case 'd':
                d = 1;
                break;
            case 's':
                s = 1;
                break;
            case 'p':
                p = 1;
                break;
            case 'i':
                protected_free(inputloc);
                inputloc = strdup(optarg);
                break;
            case 'o':
                protected_free(outputloc);
                outputloc = strdup(optarg);
                break;
            case '?': // Argument not found
                usage_message(stderr);
                exit = EXIT_FAILURE;
                goto end_program; // free all allocated memory before exiting
        }
    }
    protected_free(optarg);

    #ifdef DEBUG 
        // print what flags where flipped
        printf("d:%i s:%i p:%i i:%s o:%s\n", 
                d, s, p, inputloc, outputloc);
    #endif

    if(inputloc != NULL && (i = fopen(inputloc, "r")) == NULL) {
        perror(inputloc);
        exit = EXIT_FAILURE;
        goto end_program; // free all allocated memory before exiting
    }    
    if(outputloc != NULL && (o = fopen(outputloc, "w")) == NULL) {
        perror(outputloc);
        exit = EXIT_FAILURE;
        goto end_program; // free all allocated memory before exiting
    }
    
    Maze maze = create_maze(i); 
    if(d)
        pretty_print_maze(maze, o); 
    int steps = 0;
    if(s || p)
         steps = solve_maze(maze);
    if(s && steps > 0)
        fprintf(o, "Solution in %i steps.\n", steps);
    else if(s)
        fprintf(o, "No solution.\n");
    if(p)
        pretty_print_maze(maze, o); 

    clean_maze(maze);

    end_program:
    if(inputloc != NULL) {
        free(inputloc);
        fclose(i);
    }
    if(outputloc != NULL) {
        free(outputloc);
        fclose(o);
    }
    return exit;
}

