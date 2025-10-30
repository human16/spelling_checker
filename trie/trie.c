#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

TrieNode* make_trienode(char data) {
    TrieNode* node = (TrieNode*) calloc (1, sizeof(TrieNode));
    for (int i=0; i<ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    node->end = 0;
    node->data = data;
    return node;
}

void free_trienode(TrieNode* node) {
    for(int i=0; i<ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}

TrieNode* insert_trie(TrieNode* root, char* word) {
    TrieNode* temp = root;

    for (int i=0; word[i] != '\0'; i++) {
        // Get the relative position in the alphabet list
        int idx = (int) word[i] - '!';
        if (temp->children[idx] == NULL) {
            temp->children[idx] = make_trienode(word[i]);
        }
        temp = temp->children[idx];
    }
    temp->end = 1;
    return root;
}

int search_trie(TrieNode* root, char* word) {
    // Searches for word in the Trie
    TrieNode* temp = root;

    for(int i=0; word[i]!='\0'; i++) {
        int position = word[i] - '!';
        if (temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
    if (temp != NULL && temp->end == 1)
        return 1;
    return 0;
}
