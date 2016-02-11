#include "intersect.h"



/* Function iterates over a bst printing each element in order
 * each element in the tree is free'd as it is traversed
 * @PARAM bonsai -- bst structure to print
 * @RETURN -- zero on failure else 1
 */
int bst_prune(struct bst_tree* bonsai){
    if ( ! bonsai ){
        fprintf(stderr, "ERROR: WHAR MY TREE GO??\n");
        return 0;
    }
    if( bonsai->left ){
        bst_prune(bonsai->left);
    }
    fprintf(stdout, "%s\n", bonsai->value->word);
    if( bonsai->right ){
        bst_prune( bonsai->right );
    }
    bonsai->left = NULL;
    bonsai->right = NULL;
    free(bonsai);
    return 1;
}


/* Function inserts the element into the bst_tree in alphabetical order
 * @PARAM tree -- pointer to the tree to insert into
 * @PARAM element -- element to insert
 */
void bst_insert( struct bst_tree *tree, struct element *element){
    if ( ! tree || ! element ){
        fprintf( stderr, "ERROR: WTF MATE??? you lost you stuffs\n");
        return;
    }
    int compare = 0;
    if ( ! tree->value ){
        tree->value = element;
    }else if( ( compare = strncmp( tree->value->word, element->word, element->length ) ) < 0 ){
        bst_insert( tree->left, element );
    } else( bst_insert( tree->right, element ) );
}

/* Function removes all elements from a hashTable and inserts them into a binary 
 * search tree. Function free's all memory associated with hash_table
 * @PARAM table -- hashTable to remove items from.
 * @RETURN -- a malloc'd bst_tree containing the data in hash_table or NULL on failure
 */
struct bst_tree* hash_strip( struct hash_table* table ){
    if( ! table ){
        fprintf(stderr, "ERROR: WHAR MY HASH TABLE GO??\n");
        return NULL;
    }
    struct bst_tree* bonsai = malloc( sizeof( *bonsai ) );
    memset( bonsai, 0, sizeof( struct bst_tree ) );
    bool has_next = false;
    for ( size_t i = 0; i < table->capacity; i++ ){
        if ( table->data[i] ){
            do{
                has_next = table->data[i]->next;
                if ( table ->item_count == table->data[i]->value->count){
                    // value was in all of the files. 
                    bst_insert( bonsai, table->data[i]->value );
                }
                struct h_llist *temp = table->data[i]->next;
                table->data[i]->value = NULL;
                table->data[i]->next = NULL;
                free( table->data[i] ); 
                table->data[i] = temp; // reasign head of linked list
            }while( has_next );
        }
    }
    free( table->data );
    free( table );
    return bonsai;
}


/* hashing function provided by WECHLIN 
 * @PARAM key -- value to be hashed
 * @RETURN -- a unique value based on the input to be used as a hash index.
 */
uint64_t wang_hash(char* buf){
    uint64_t key = 0;
	strncpy((char *)(&key), buf, sizeof(key));
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	key = key ^ (key >> 28);
	key = key + (key << 31);
	return key;
}


/* Function inserts element into the hash table using the string 
 * contained in element as the key. 
 * @PARAM element -- the structure to be inserted into the hash_table
 * @PARAM data -- the hash_table to insert the value into
 * @RETURN -- returns 0 on failure, else 1
 */
int hash_insert(struct element *value, struct hash_table* data){
    if ( ! value || ! data ) {
        //error
    }
    //hash value of element
    //check address of hash 
    //if data[hash] && data[hash]->count == element->count-1
        //data[hash]->count = element->count
    //else if( count == 1)
        //data[hash] = element
    return 1;
}


/* Function opens the specified file and inserts each word in the file into the hash
 * table.
 * @PARAM data -- hash table to insert data into
 * @PARAM filename -- name of file to open
 * @RETURN -- 0 on failure else 1;
 */
int run(struct hash_table* data, const char* filename){
    if ( ! data || ! filename ){
        //error 
    }
    // open file for reading 
    // create value structure 
    // insert each element into hash table
    // close file
    return 1;
}


/* Function exectuion begins here. Useage is ./intersect FILENAME FILENAME ...
 * Program takes multiple files and finds the words taht are common between
 * all files. Once found all common words are printed lexographically. A word is
 * delimited by whitespace, search is case insesnitive. Printed words will match 
 * the case of the first instance they are found.
 * @PARAM argc -- number of arguments in argv
 * @PARAM argv -- array of filenames to be compared
 * @RETURN -- returns 0 on failure, else 1
 */
int main(int argc, char*argv[]){
    if( argc < 2 || argv == NULL ){
        //error message
    }
    // verify input
    // allocate space for hash_table
    // do while count = 1 < argc-1
        //run()
        //count ++
    // bst = hash_strip
    // print bst
    // exit
}