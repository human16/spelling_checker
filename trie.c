#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 52 lowercase and uppercase english characters
#define ALPHABET_SIZE 52

typedef struct TrieNode TrieNode;

struct TrieNode {
    char data; // for printing purposes
    TrieNode* children[ALPHABET_SIZE];
    int end;
};

// Allocate memory for a TrieNode
TrieNode* make_trienode(char data) {
    TrieNode* node = (TrieNode*) calloc (1, sizeof(TrieNode));
    for (int i=0; i<ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    node->end = 0;
    node->data = data;
    return node;
}

// Free the trienode sequence
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

// Inserts the word onto the Trie
TrieNode* insert_trie(TrieNode* root, char* word) {
    TrieNode* temp = root;

    for (int i=0; word[i] != '\0'; i++) {
        // Get the relative position in the alphabet list
        int idx = (int) word[i] - 'A';
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
        int position = word[i] - 'A';
        if (temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
    if (temp != NULL && temp->end == 1)
        return 1;
    return 0;
}

void print_trie(TrieNode* root) {
    // Prints the nodes of the trie
    if (!root)
        return;
    TrieNode* temp = root;
    printf("%c -> ", temp->data);
    for (int i=0; i<ALPHABET_SIZE; i++) {
        print_trie(temp->children[i]); 
    }

    printf("\n");
}

void print_search(TrieNode* root, char* word) {
    printf("Searching for %s: ", word);
    if (search_trie(root, word) == 0)
        printf("Not Found\n");
    else
        printf("Found!\n");
}

// int main() {
//     TrieNode* root = make_trienode('\0');
//     root = insert_trie(root, "Hello");
//     root = insert_trie(root, "hI");
//     root = insert_trie(root, "teaBag");
//     root = insert_trie(root, "teacan");
//     print_search(root, "tea"); // Expected not found
//     print_search(root, "teaBag"); // Expected found
//     print_search(root, "teacan"); // Expected found
//     print_search(root, "hI"); // Expected found
//     print_search(root, "hey"); // Expeceted not found
//     print_search(root, "Hello"); // Expecetd found
//     print_trie(root);
//     free_trienode(root);
//     return 0;
// }
