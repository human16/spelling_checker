#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef DEBUG
#define DEBUG 1
#endif

//will be used to count the number of times . or / repeat in the argument
int count_char(char *arg, char find_char) {
    int out = 0;
    for (int i = 0; arg[i] != "\0"; i++) {
        if (arg[i] == find_char) {
            out++;
        }
    }
    return out;
}

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