#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct h_llist {
	//char *key;
	struct element *value;
	struct h_llist *next;
};

struct hash_table{
	size_t item_count;
	size_t capacity;
	struct h_llist **data;
};

struct element{
    size_t length, count;
    const char* word;
};

struct bst_tree{
    struct element *value;
    struct bst_tree *left, *right;
};


int bst_prune(struct bst_tree* bonsai);
void bst_insert( struct bst_tree *tree, struct element *element);
struct bst_tree* hash_strip(struct hash_table* data);
uint64_t wang_hash(char* buf);
int hash_insert(struct element *value, struct hash_table* data);
int run(struct hash_table* data, const char* filename);