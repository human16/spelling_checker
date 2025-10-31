#include "searcher/searcher.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 1
#endif

char *get_file_extension(char *filename) {
  char *dot = strrchr(filename, '.');

  if (!dot || dot == filename || *(dot + 1) == '\0') {
    return NULL;
  }
  return dot + 1;
}

int scan_file(char *file) {
  if (DEBUG) {
    printf("scan file \"%s\"\n", file);
  }

  int fd = open(file, O_RDONLY);
  if (fd == -1) {
    printf("There was a big no no in file \"%s\"\n", file);
    return EXIT_FAILURE;
  }

  int buffer_size = 16;
  char *buffer = malloc(buffer_size * sizeof(char));
  int line = 1;
  int col = 0;
  int bytes;
  int linestart = 0;
  while ((bytes = read(fd, buffer + col, buffer_size - col)) > 0) {
    // getting the end for this specific itteration
    int end = col + bytes;

    // resetting linestart at the beginning of a new line
    linestart = 0;
    if (DEBUG) {
      printf("got %d bytes at pos %d with end %d\n", bytes, col, end);
    }
    for (; col < end; col++) {
      if (*(buffer + col) == '\n' || *(buffer + col) == '\0' ||
          *(buffer + col) == ' ') {
        if (DEBUG) {
          printf("Stopped at col: %d\n", col);
        }
        int linelen = col - linestart;
        if (linelen > 0) {
          char *word = malloc((size_t)linelen + 1);
          if (!word) {
            free(buffer);
            close(fd);
            return EXIT_FAILURE;
          }
          memcpy(word, buffer + linestart, (size_t)linelen);
          word[linelen] = '\0';
          if (DEBUG) {
            printf("character: %d/%d is string: %s with length: %d\n",
                   linestart, col, word, linelen);
          }

          // search_word is provided with linelen-1 instead of linelen to ommit
          // the '\0'
          if (search_word(word, linelen - 1, line, linestart, file)) {
            return EXIT_FAILURE;
          }
          free(word);
        }
        linestart = col + 1;
        if (*(buffer + col) == '\n')
          line++;
      }
    }

    // The word is too large to fit in the buffer, so buffer size is doubled!
    if (linestart == 0) {
      int new_size = buffer_size * 2;
      char *newbuf = realloc(buffer, (size_t)new_size);
      if (!newbuf) {
        free(buffer);
        close(fd);
        return EXIT_FAILURE;
      }
      buffer = newbuf;
      buffer_size = (int)new_size;
      if (DEBUG)
        printf("[size -> %d]\n", buffer_size);
    }
    if (linestart < col) {
      int linelen = col - linestart;
      memmove(buffer, buffer + linestart, linelen);
      col = linelen;
      linestart = 0;
      if (DEBUG)
        printf("shift %d bytes\n", linelen);
    } else {
      col = 0;
    }
  }

  // if the file ends with a word
  if (col > 0 && linestart < col) {
    int linelen = col - linestart;
    char *word = malloc((linelen + 1) * sizeof(char));
    if (word) {
      memcpy(word, buffer + linestart, linelen);
      word[linelen] = '\0';
      if (DEBUG)
        printf("[line: %d/%d] String: %s with length: %d\n", linestart, col,
               word, linelen);
      if (search_word(word, linelen, line, linestart)) {
        return EXIT_FAILURE;
      }
      free(word);
    }
  }
  close(fd);
  free(buffer);
  return EXIT_SUCCESS;
  // implement
}

int scan_directory(char *directory, char *suffix) {
  if (DEBUG) {
    printf("scan directory \"%s\" with suffix \"%s\"\n", directory, suffix);
  }
  DIR *dirp;
  struct dirent *dp;

  dirp = opendir(directory);

  if (dirp == NULL) {
    printf("opendir failed");
    return 1;
  }
  while ((dp = readdir(dirp)) != NULL) {
    /* skip current and parent directories */
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
      continue;
    }

    /* build full path: directory + '/' + d_name */
    size_t dirlen = strlen(directory);
    size_t namelen = strlen(dp->d_name);
    char *path = malloc(dirlen + 1 + namelen + 1);
    if (!path) {
      if (DEBUG)
        printf("malloc failed for path\n");
      closedir(dirp);
      return EXIT_FAILURE;
    }
    memcpy(path, directory, dirlen);
    path[dirlen] = '/';
    memcpy(path + dirlen + 1, dp->d_name, namelen);
    path[dirlen + 1 + namelen] = '\0';

    if (DEBUG) {
      printf("working on %s\n", path);
    }

    struct stat file_info;
    if (stat(path, &file_info) != 0) {
      if (DEBUG) {
        printf("stat failed, path = %s\n", path);
      }
      free(path);
      closedir(dirp);
      return EXIT_FAILURE;
    }

    if (S_ISREG(file_info.st_mode)) {
      /* check suffix: suffix expected like ".txt"; match file end */
      size_t slen = strlen(suffix);
      if (slen == 0 || (namelen >= slen &&
                        strcmp(dp->d_name + namelen - slen, suffix) == 0)) {
        if (scan_file(path)) {
          free(path);
          closedir(dirp);
          return EXIT_FAILURE;
        }
      }
    } else if (S_ISDIR(file_info.st_mode)) {
      if (scan_directory(path, suffix)) {
        free(path);
        closedir(dirp);
        return EXIT_FAILURE;
      }
    }

    free(path);
  }

  closedir(dirp);
  return EXIT_SUCCESS;
  // implement
}

int scan_input() {
  int buffer_size = 16;
  char *buffer = malloc(buffer_size * sizeof(char));
  int line = 1;
  int col = 0;
  int bytes;
  int linestart = 0;
  while ((bytes = read(STDIN_FILENO, buffer + col, buffer_size - col)) > 0) {
    // getting the end for this specific itteration
    int end = col + bytes;

    // resetting linestart at the beginning of a new line
    linestart = 0;
    if (DEBUG) {
      printf("got %d bytes at pos %d with end %d\n", bytes, col, end);
    }
    for (; col < end; col++) {
      if (*(buffer + col) == '\n' || *(buffer + col) == '\0' ||
          *(buffer + col) == ' ') {
        if (DEBUG) {
          printf("Stopped at col: %d\n", col);
        }
        int linelen = col - linestart;
        if (linelen > 0) {
          char *word = malloc((size_t)linelen + 1);
          if (!word) {
            free(buffer);
            return EXIT_FAILURE;
          }
          memcpy(word, buffer + linestart, (size_t)linelen);
          word[linelen] = '\0';
          if (DEBUG) {
            printf("character: %d/%d is string: %s with length: %d\n",
                   linestart, col, word, linelen);
          }

          // search_word is provided with linelen-1 instead of linelen to ommit
          // the '\0'
          if (search_word(word, linelen - 1, line, linestart)) {
            return EXIT_FAILURE;
          }
          free(word);
        }
        linestart = col + 1;
        if (*(buffer + col) == '\n')
          line++;
      }
    }

    // The word is too large to fit in the buffer, so buffer size is doubled!
    if (linestart == 0) {
      int new_size = buffer_size * 2;
      char *newbuf = realloc(buffer, (size_t)new_size);
      if (!newbuf) {
        free(buffer);
        return EXIT_FAILURE;
      }
      buffer = newbuf;
      buffer_size = (int)new_size;
      if (DEBUG)
        printf("[size -> %d]\n", buffer_size);
    }
    if (linestart < col) {
      int linelen = col - linestart;
      memmove(buffer, buffer + linestart, linelen);
      col = linelen;
      linestart = 0;
      if (DEBUG)
        printf("shift %d bytes\n", linelen);
    } else {
      col = 0;
    }
  }

  // if the file ends with a word
  if (col > 0 && linestart < col) {
    int linelen = col - linestart;
    char *word = malloc((linelen + 1) * sizeof(char));
    if (word) {
      memcpy(word, buffer + linestart, linelen);
      word[linelen] = '\0';
      if (DEBUG)
        printf("[line: %d/%d] String: %s with length: %d\n", linestart, col,
               word, linelen);
      if (search_word(word, linelen, line, linestart)) {
        return EXIT_FAILURE;
      }
      free(word);
    }
  }
  free(buffer);
  return EXIT_SUCCESS;
}
