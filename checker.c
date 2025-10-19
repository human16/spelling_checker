#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef DEBUG
#define DEBUG 1
#endif

int read_inputs(int argc, char *argv[], char *suffix, char **files, char **directories) {
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
        }
        
    }

    return 0;
}


int main(int argc, char *argv[]) {
    char *suffix = ".txt";
    char **files = malloc((argc - 1)*sizeof(char *)); // assuming all arguments are files
    char **directories = malloc((argc - 1)*sizeof(char *)); // assuming all arguments are directories
    int result = read_inputs(argc, argv, suffix, files, directories);
    if (result != 0) {
        free(files);
        free(suffix);
        free(directories);
        return 1;
    }


    return 0;
}