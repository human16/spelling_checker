#include "read_dict.h"
#include "scanner.h"
#include "searcher/searcher.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 1
#endif

// will be used to count the number of times . or / repeat in the argument
int count_char(char *arg, char find_char) {
  int out = 0;
  for (int i = 0; arg[i] != '\0'; i++) {
    if (arg[i] == find_char) {
      out++;
    }
  }
  return out;
}

int parse(int argc, char *argv[]) {
  char *suffix;
  if (argc == 2) {
    struct stat file_info;
    char *dictionary = argv[1];
    if (DEBUG) {
      printf("Dictionary: %s\n", dictionary);
    }
    if (stat(argv[1], &file_info) != 0) {
      printf("stat failed, argv[i] = %s\n", argv[1]);
      return 1;
    }
    if (S_ISREG(file_info.st_mode)) {
      TrieNode *dict_root = read_dictionary(dictionary);

      if (dict_root == NULL) {
        if (DEBUG) {
          printf("Failed to load dictionary from %s\n", dictionary);
        }
        return EXIT_FAILURE;
      }

      set_dict(dict_root);

      if (DEBUG) {
        printf("Dictionary loaded successfully\n");
      }
    } else {
      printf("directory can't be a dictionary");
      return 1;
    }
    return scan_input();
  }
  int i = 1;
  if (!strcmp(argv[1], "-s")) {
    i = 3;
    if (argc > 2) {
      if (argv[2][0] == '.' && count_char(argv[2], '.') == 1) {
        if (DEBUG) {
          printf("suffix is %s", argv[2]);
        }
        suffix = argv[2];
      } else {
        if (DEBUG) {
          printf(
              "format of suffix is invalid. First char %c, number of '.'s %d\n",
              argv[2][0], count_char(argv[2], '.'));
        }
        return 1;
      }
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
    if (DEBUG) {
      printf("default suffix is .txt\n");
    }
    suffix = ".txt";
  }
  struct stat file_info;
  char *dictionary = argv[i];
  if (DEBUG) {
    printf("Dictionary: %s\n", dictionary);
  }
  if (stat(argv[i], &file_info) != 0) {
    printf("stat failed, argv[i] = %s\n", argv[i]);
    return 1;
  }
  if (S_ISREG(file_info.st_mode)) {
    TrieNode *dict_root = read_dictionary(dictionary);

    if (dict_root == NULL) {
      if (DEBUG) {
        printf("Failed to load dictionary from %s\n", dictionary);
      }
      return EXIT_FAILURE;
    }

    set_dict(dict_root);

    if (DEBUG) {
      printf("Dictionary loaded successfully\n");
    }
  } else {
    printf("directory can't be a dictionary");
    return 1;
  }

  // char *dictionary = argv[i];
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
      if (scan_file(argv[i])) {
        return EXIT_FAILURE;
      }
    } else if (S_ISDIR(file_info.st_mode)) {
      if (scan_directory(argv[i], suffix)) {
        return EXIT_FAILURE;
      }
    }

    i++;
  }
  return 0;
}
