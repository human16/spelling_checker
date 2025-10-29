#ifndef TRIE_H
#define TRIE_H

// include lowercase and uppercase
// english letters.
#define ALPHABET_SIZE 52

typedef struct TrieNode TrieNode;
struct TrieNode {
  char data;
  TrieNode* children[ALPHABET_SIZE];
  int leaf;
};

TrieNode* make_trienode(char data);
void free_trienode(TrieNode* node);
TrieNode* insert_trie(TrieNode* root, char* word);
int search_trie(TrieNode* root, char* word);
void print_trie(TrieNode* root);
void print_search(TrieNode* root, char* word);

#endif
