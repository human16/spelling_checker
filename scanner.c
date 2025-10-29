#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

#ifndef DEBUG
#define DEBUG 1
#endif

int scan_file(char *file) {
    if (DEBUG) {
        printf("scan file \"%s\"\n", file);
    }

    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        printf("There was a big no no in file \"%s\"", file);
    }

    int buffer_size = 16;
    char *buffer = malloc(buffer_size * sizeof(char));
    int line = 1;
    int col = 1;
    while ((read(fd, buffer, buffer_size)) > 0) {
        
    }
    free(buffer);


    return EXIT_SUCCESS;
    //implement
}

int scan_directory(char *directory, char *suffix) {
    if (DEBUG) {
        printf("scan directory \"%s\" with suffix \"%s\"\n", directory, directory);
    }
    return EXIT_SUCCESS;
    //implement
}

int scan_input() {
    if (DEBUG) {
        printf("scan inputs\n");
    }
    return EXIT_SUCCESS;
    //implement
}