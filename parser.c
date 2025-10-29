#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "scanner.h"

#ifndef DEBUG
#define DEBUG 1
#endif

int parse(int argc, char *argv[]) {
    char *suffix;
    if (argc == 2) {
        return scan_input();
    }
    int i = 1;
    if (!strcmp(argv[1], "-s")) {
        i = 3;
        if (argc > 2) {
            suffix = argv[2];
        } else {
            if (DEBUG) {
                printf("no suffix given after -s flag\n");
            }
            return 1;
        }
        if (argc < 4) {
            if (DEBUG) {
                printf("not enough inputs\n");
            }
            return 1;
        }
    } else {
        suffix = ".txt";
    }
    struct stat file_info;
    char *dictionary = argv[i];
    if (stat(argv[i], &file_info) != 0) {
        printf("stat failed, argv[i] = %s\n", argv[i]);
        return 1;
    }
    if (S_ISREG(file_info.st_mode)) {
        //insert trie population function
    } else {
        printf("directory is not a dictionary");
        return 1;
    }

    //char *dictionary = argv[i];
    i++;
    while (i < argc) {
        struct stat file_info;
        
        if (DEBUG) {
            printf("working on %s\n", argv[i]);
        }

        if (stat(argv[i], &file_info) != 0) {
            printf("stat failed, argv[i] = %s\n", argv[i]);
            return 1;
        }
        
        if (S_ISREG(file_info.st_mode)) {
            scan_file(argv[i]);
        } else if (S_ISDIR(file_info.st_mode)) {
            scan_directory(argv[i], suffix);
        }

        i++;
    }
    return 0;
}