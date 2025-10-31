#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "parser.h"

#ifndef DEBUG
#define DEBUG 1
#endif

/*int read_inputs(int argc, char *argv[], char *suffix, char *files[], char *directories[], char *dictionary) {
    if (argc < 3) {
        printf("Missing arguments");
        return 1;
    }
    //making sure the suffix exists
    int suffix_arg_exists = 0;
    if (!strcmp(argv[1], "-s")) {
        suffix_arg_exists = 1;
        if (*argv[2] != '.') {
            printf("Incorrect suffix format");
            return 1;
        }
        suffix = argv[2];
    }

    //starting after the index after the suffix if it exists 
    for (int i = 1 + 2 * (suffix_arg_exists); i < argc; i++) {
        if (!strcmp(argv[i], "-s")) {
            printf("-s appears in a place that's not the first position");
            return 1;
        }

        int num_per = count_char(argv[i], ".");
        if (num_per == 0) {
            strcat(argv[i], suffix);
        } else if (num_per > 1) {
            printf("too many periods (.) in one argument");
            return 1;
        }
        if (count_char(argv[i], "/")) {
            return 0;
        }
        
    }

    return 0;
}
*/

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("no arguments\n");
        return EXIT_FAILURE;
    }
    if (parse(argc, argv)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}