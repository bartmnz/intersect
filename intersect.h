#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct h_llist {
	char *key;
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
struct bst_tree* hash_strip(struct hash_table* data);
uint64_t wang_hash(uint64_t key);
int hash_insert(struct element *value, struct hash_table* data);
int run(struct hash_table* data, const char* filename);