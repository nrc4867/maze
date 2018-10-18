/// file: mopsolver.c
/// description: solves mazes
/// author: Nicholas R. Chieppa
///

#define _DEFAULT_SOURCE

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
    fprintf(stream, "USAGE:\nmopsolver [-hdsp] [-i INFILE] [-o OUTFILE]");
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
    
    // input and output locations should only be changed after
    // all arguments are read, protects against multiple uses of -i/-o
    char *inputloc = ""; 
    char *outputloc = ""; 
    
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
                inputloc = strdup(optarg);
                break;
            case 'o':
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

    if(strlen(inputloc) > 0 && (i = fopen(inputloc, "r")) == NULL) {
        perror(inputloc);
        exit = EXIT_FAILURE;
        goto end_program; // free all allocated memory before exiting
    }    
    if(strlen(outputloc) > 0 && (o = fopen(outputloc, "r")) == NULL) {
        perror(outputloc);
        exit = EXIT_FAILURE;
        goto end_program; // free all allocated memory before exiting
    }
    
    
    
    end_program:
    free(inputloc);
    free(outputloc);
    protected_free(i);
    protected_free(o);
    return exit;
}

