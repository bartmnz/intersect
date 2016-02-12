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
    if ( ! tree->value ){
        tree->value = element;
    }else if(  strncmp( tree->value->word, element->word, element->length )  > 0 ){
        if ( tree->left ){
            bst_insert( tree->left, element );
        }else {
            tree->left = malloc( sizeof( *tree->left ) );
            if ( ! tree->left ){
                fprintf(stderr, "ERROR: malloc failed \n");
                return;
            }
            memset( tree->left, 0, sizeof( *tree->left ) );
            tree->left->value = element;
        }
        
    } else if ( tree->right ){
            bst_insert( tree->right, element );
    }else {
        tree->right = malloc( sizeof( *tree->right ) );
        if ( ! tree->right ){
            fprintf(stderr, "ERROR: malloc failed \n");
            return ;
        }
        memset( tree->right, 0, sizeof( *tree->right ) );
        tree->right->value = element;
    }
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
    if ( ! bonsai ){
        fprintf(stderr, "ERROR: malloc failed \n");
        return NULL;
    }
    memset( bonsai, 0, sizeof( *bonsai ) );
    
    printf("here1234\n");
    
    struct h_llist *temp = NULL; 
    struct h_llist *current = NULL;
    printf("table capacity is (%zu)\n",table->capacity);
    for ( size_t i = 0; i < table->capacity; i++ ){
        printf("i (%zu)", i);
        if ( ( current = table->data[i] ) ){
            printf(" data[%zu]", i);
            do{
                if ( table->file_count == current->value->count){
                    // value was in all of the files. 
                    printf("inserting %s\n", current->value->word);
                    bst_insert( bonsai, current->value );
                    
                }
                temp = current->next;
                current->value = NULL;
                current->next = NULL;
                free( current ); 
                table->data[i] = temp; // reasign head of linked list
            }while( temp );
        }
        
    }
    
    if ( table ){
        if ( table->data ){
            free( table->data );
        }
        free( table );
    }
    return bonsai;
}


/* Function creates a unique hash for an element : is case insensitive
 * hashing function provided by WECHLIN 
 * @PARAM value -- value to be hashed
 * @RETURN -- a unique value based on the input to be used as a hash index.
 */
uint64_t wang_hash(struct element* value){
    uint64_t key = 0;
    char* buf = malloc ( sizeof( char ) * value->length );
    if ( ! buf ){
        fprintf(stderr, "ERROR: malloc failed \n");
        return 0;
    }
    for( size_t i = 0; i < value->length; i++ ){
        buf[i] = tolower( value->word[i] );
    }
	strncpy((char *)(&key), buf, sizeof(key));
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	key = key ^ (key >> 28);
	key = key + (key << 31);
	free( buf );
	return key;
}

/* Function compares two strings case independent
 * @PARAM value -- structure containing first string to be checked
 * @PARAM string2 -- second string to be checked
 * @RETURN -- true if strings are the same
 */
bool same_word(struct element* value, char *string2){
    if ( ! value || ! string2 ){
        fprintf( stderr, "ERROR: aborting\n");
        return false;
    }
    char* buf = malloc ( sizeof( char ) * value->length );
    if ( ! buf ){
        fprintf( stderr, "ERROR: aborting\n");
        return false;
    }
    for( size_t i = 0; i < value->length; i++ ){
        buf[i] = tolower( value->word[i] );
    }
    free( buf );
    
    return !strncmp( value->word, string2, value->length );
}


/* Function inserts element into the hash table using the string 
 * contained in element as the key. 
 * @PARAM element -- the structure to be inserted into the hash_table
 * @PARAM table -- the hash_table to insert the value into
 * @RETURN -- returns 0 on failure, else 1
 */
int hash_insert(struct element *element, struct hash_table* table){
    if ( ! element || ! element->word || ! table || ! table->data) {
        fprintf(stdout, "ERROR: This message should probably be more helpful\n");
        return 0;
    }
    int index = 0;
    index = wang_hash(element) % table->capacity;  //hash element of element
    struct h_llist* hash = table->data[index]; 
    if ( hash ){
        do{
            if( same_word( element, hash->value->word ) ){ //got the same word
                if ( hash->value->count + 1 == element->count ){
                    hash->value->count++;
                }
            }
            hash = hash->next;
        }while ( hash );
    }else if ( table->file_count == 1 ){ //nothing at the index and first file
        hash = malloc ( sizeof( *hash ) );
        if ( !hash ){
            fprintf(stdout, "ERROR: This message should probably be more helpful\n");
            return 0;
        }
        memset( hash, 0, sizeof( *hash ) );
        hash->value = element;
        table->data[index] = hash;
    }
    return 1;
}


/* Function opens the specified file and inserts each word in the file into the hash
 * table.
 * @PARAM data -- hash table to insert data into
 * @PARAM filename -- name of file to open
 * @RETURN -- 0 on failure else 1;
 */
int run(struct hash_table* table, const char* filename){
    if ( ! table || ! filename ){
        fprintf( stderr, "ERROR: Yo DAWG i heard you like seg-faults so i ....\n ");
        return 0;
    }
    FILE* file;
    if ( ! ( file = fopen( filename, "r" ) ) ){ // open file for reading 
        fprintf(stderr, "ERROR: could not open file. Check filename and permissions.\n");
        return 0;
    }
    table->file_count++; // keep track of the number of files we have looked at
    struct element* my_element;
    long temp;
    char star;
    int count = 0;
    while( true ){
        my_element = malloc( sizeof( *my_element ) );
        if ( ! my_element ){
            fprintf(stderr, "ERROR: malloc failed \n");
            exit(0);
        }
        memset( my_element, 0, sizeof( *my_element ) );
        my_element->count = table->file_count;
        temp = ftell( file );
        while ( ( star = fgetc( file ) )!= EOF && !isspace( star ) ){
            count++;
        }
        fseek( file, temp, SEEK_SET );
        my_element->word = malloc( sizeof ( char ) * count );
        if ( ! my_element->word ){
            fprintf(stderr, "ERROR: malloc failed \n");
            exit(0);
        }
        my_element->length = count;
        fscanf( file, "%s", my_element->word );
        //printf("%s\n", my_element->word);
        hash_insert( my_element, table );
        if( star == EOF ){
            //printf("end\n");
            break;
        }
    }
    fclose(file);
    return 1;
}


/* Function opens a file and counts the number of white spaces in the file to 
 * determine how many words are in the file see isspace(3) for whitespace
 * @PARAM filename -- name of file to be opened
 * @RETURN -- number of whitespaces in the file 
 */
size_t how_big(const char* filename){
    if ( ! filename ){
        fprintf( stderr, "ERROR: Yo DAWG i heard you like seg-faults so i ....\n ");
        return 0;
    }
    FILE* file;
    if ( ! ( file = fopen( filename, "r" ) ) ){ // open file for reading 
        fprintf(stderr, "ERROR: could not open file. Check filename and permissions.\n");
        return 0;
    }
    char star;
    size_t count = 0;
    while ( ( star = fgetc( file ) )!= EOF ){
        if ( isspace( star ) ){
            count++;
        }
    }
    fclose(file);
    //printf("%zu\n", count );
    return count + count % 3;
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
    if ( argc < 2 ){
        fprintf(stderr, "ERROR: useage is ./intersect FILENAME FILENAME ...\n");
        exit(0);
    }
    struct hash_table* table = malloc ( sizeof( *table ) );
    if ( ! table ){
        fprintf(stderr, "ERROR: malloc failed \n");
        exit(0);
    }
    memset( table, 0, sizeof( *table ) );
    table->capacity = how_big( argv[1] );
    table->data = malloc( sizeof( *table->data ) * table->capacity );
    if ( ! table->data ){
        fprintf(stderr, "ERROR: malloc failed \n");
        exit(0);
    }
    memset( table->data, 0, sizeof( *table->data ) * table->capacity );
    for ( int i = 1; i < argc; i++ ){
        run( table, argv[i] );
    }
    printf("table capacity is (%zu)\n",table->capacity);
    struct bst_tree* bonsai = hash_strip(table);
    bst_prune(bonsai);
}