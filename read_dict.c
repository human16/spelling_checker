#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 1
#endif

TrieNode *read_dictionary(char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    if (DEBUG) {
      printf("Failed to open dictionary file: %s\n", filename);
    }
    return NULL;
  }

  TrieNode *root = make_trienode('\0');

  int bufsize = 16;
  char *buf = malloc(bufsize);
  if (buf == NULL) {
    close(fd);
    return NULL;
  }

  int pos = 0;
  int bytes;

  while ((bytes = read(fd, buf + pos, bufsize - pos)) > 0) {
    int linestart = 0;
    int end = pos + bytes;
    if (DEBUG)
      printf("[got %d, pos %d, end %d]\n", bytes, pos, end);

    while (pos < end) {
      if (buf[pos] == '\n') {
        int linelen = pos - linestart;
        if (DEBUG)
          printf("[line: %d/%d]\n", linestart, pos);

        buf[pos] = '\0';

        if (linelen > 0) {
          root = insert_trie(root, buf + linestart);
        }

        linestart = pos + 1;
      }
      pos++;
    }

    if (linestart == 0) {
      // Grow buffer if no newline is found
      bufsize *= 2;
      buf = realloc(buf, bufsize);
      if (DEBUG)
        printf("[size -> %d]\n", bufsize);
    } else if (linestart < pos) {
      int linelen = pos - linestart;
      memmove(buf, buf + linestart, linelen);
      pos = linelen;
      if (DEBUG)
        printf("[shift %d bytes]\n", linelen);
    } else {
      pos = 0;
    }
  }

  if (pos > 0) {
    buf[pos] = '\0';
    root = insert_trie(root, buf);
  }

  free(buf);
  close(fd);

  return root;
}
