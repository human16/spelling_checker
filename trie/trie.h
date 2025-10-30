#ifndef TRIE_H
#define TRIE_H

// accounts for 93 ascii characters, from the range [33, 126]
#define ALPHABET_SIZE 93

typedef struct TrieNode TrieNode;
struct TrieNode {
  char data;
  TrieNode* children[ALPHABET_SIZE];
  int end;
};

// TrieNode* make_trienode(char data);
// void free_trienode(TrieNode* node);
TrieNode* insert_trie(TrieNode* root, char* word);
int search_trie(TrieNode* root, char* word);

#endif
