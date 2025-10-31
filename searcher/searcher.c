#include "trie/trie.h"
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 1
#endif

TrieNode *dict_root = NULL;

void set_dict(TrieNode *dict) { dict_root = dict; }

char *strip_non_letters(char *word, int len) {
  if (word == NULL || len <= 0) {
    return NULL;
  }

  int start = 0;
  int end = len - 1;

  while (start < len && !isalpha(word[start])) {
    start++;
  }

  if (start >= len) {
    return NULL; // didn't find any letters
  }

  while (end >= start && !isalpha(word[end])) {
    end--;
  }

  int cleaned_len = end - start + 1;
  char *cleaned_word;

  cleaned_word = (char *)malloc(cleaned_len + 1); // +1 for '\0'
  if (cleaned_word == NULL) {
    if (DEBUG) {
      printf("Memory allocation failed for cleaned_word\n");
    }
    return NULL;
  }

  memcpy(cleaned_word, word + start, cleaned_len);
  cleaned_word[cleaned_len] = '\0';

  return cleaned_word;
}

int search_word(char *word, int len, int line, int col, char *file_name) {
  if (DEBUG) {
    printf("Searching for word: %s, len: %d, on line: %d col: %d in %s\n", word,
           len, line, col, file_name);
  }

  if (dict_root == NULL) {
    if (DEBUG) {
      printf("Error, global dicitonary is null\n");
    }
    return EXIT_FAILURE;
  }

  char *cleaned_word = strip_non_letters(word, len);

  if (cleaned_word == NULL) {
    if (DEBUG) {
      printf("cleaned_word is NULL, word had no letters\n");
    }
    return EXIT_SUCCESS; // should we count this as a failure?
  }

  if (search_trie(dict_root, cleaned_word)) {
    free(cleaned_word);
    return EXIT_SUCCESS;
  } else {
    printf("%s:%d:%d %s\n", file_name, line, col, cleaned_word);
    free(cleaned_word);
    return EXIT_SUCCESS;
  }
}
