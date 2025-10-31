#include "../trie/trie.h"
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

char *normalize_word(char *word) {
  if (word == NULL) {
    return NULL;
  }

  int len = strlen(word);
  char *normalized = (char *)malloc(len + 1);
  if (normalized == NULL) {
    return NULL;
  }

  for (int i = 0; i < len; i++) {
    normalized[i] = tolower(word[i]);
  }
  normalized[len] = '\0';

  return normalized;
}

int is_capitalization_valid(char *input_word, char *dict_word) {
  if (input_word == NULL || dict_word == NULL) {
    return 0;
  }

  int len = strlen(input_word);
  if (len != strlen(dict_word)) {
    return 0;
  }

  for (int i = 0; i < len; i++) {
    // if dictionary has uppercase, input MUST have uppercase
    if (isupper(dict_word[i]) && !isupper(input_word[i])) {
      return 0;
    }
    // if dictionary has lowercase, input can be either case
    // just need to verify they're the same letter
    if (tolower(input_word[i]) != tolower(dict_word[i])) {
      return 0;
    }
  }

  return 1;
}

int search_trie_with_capitalization(TrieNode *node, char *input_word,
                                    char *dict_word_so_far, int depth) {
  if (node == NULL) {
    return 0;
  }

  if (input_word[depth] == '\0') {
    if (node->end == 1) {
      if (is_capitalization_valid(input_word, dict_word_so_far)) {
        return 1;
      }
    }
    return 0;
  }

  char input_lower = tolower(input_word[depth]);
  char input_upper = toupper(input_word[depth]);

  // build the dictionary word
  int new_len = depth + 2; // +1 for new char, +1 for null terminator
  char *new_dict_word = (char *)malloc(new_len);
  if (new_dict_word == NULL) {
    return 0;
  }
  if (depth > 0) {
    memcpy(new_dict_word, dict_word_so_far, depth);
  }

  // check lowercase in trie
  int idx_lower = input_lower - '!';
  if (idx_lower >= 0 && idx_lower < ALPHABET_SIZE &&
      node->children[idx_lower] != NULL) {
    new_dict_word[depth] = input_lower;
    new_dict_word[depth + 1] = '\0';
    if (search_trie_with_capitalization(node->children[idx_lower], input_word,
                                        new_dict_word, depth + 1)) {
      free(new_dict_word);
      return 1;
    }
  }

  // check uppercase in trie
  int idx_upper = input_upper - '!';
  if (idx_upper >= 0 && idx_upper < ALPHABET_SIZE &&
      node->children[idx_upper] != NULL) {
    new_dict_word[depth] = input_upper;
    new_dict_word[depth + 1] = '\0';
    if (search_trie_with_capitalization(node->children[idx_upper], input_word,
                                        new_dict_word, depth + 1)) {
      free(new_dict_word);
      return 1;
    }
  }

  free(new_dict_word);
  return 0;
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

  // try exact match
  if (search_trie(dict_root, cleaned_word)) {
    free(cleaned_word);
    return EXIT_SUCCESS;
  }

  // case-insensitive search with capitalization
  char *empty_str = (char *)malloc(1);
  if (empty_str != NULL) {
    empty_str[0] = '\0';
    if (search_trie_with_capitalization(dict_root, cleaned_word, empty_str,
                                        0)) {
      free(empty_str);
      free(cleaned_word);
      return EXIT_SUCCESS;
    }
    free(empty_str);
  }
  if (file_name == NULL) {
    printf("stdin:%d:%d %s\n", line, col, cleaned_word);
    free(cleaned_word);
    return EXIT_SUCCESS;
  }
  printf("%s:%d:%d %s\n", file_name, line, col, cleaned_word);
  free(cleaned_word);
  return EXIT_SUCCESS;
}
