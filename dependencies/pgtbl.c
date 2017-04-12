// implementation modified from https://gist.github.com/tonious/1377667
#include "pgtbl.h"

/* Create a new hashtable. */
hashtable_t *ht_create( int size, , ffl_t * f ) {

	hashtable_t *hashtable = NULL;
	int i;

	if( size < 1 ) return NULL;

	/* Allocate the table itself. */
	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;
    hashtable->framelist = f; 

	return hashtable;	
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, page_t *value ) {
	entry_t *newpair;

	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->key = strdup( key ) ) == NULL ) {
		return NULL;
	}

	page_t * valuecpy = malloc(sizeof(page_t)); 
	valuecpy = value; 
	newpair->value = valuecpy; 

	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, page_t *value ) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = ht_hash( hashtable, key );
	
	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		free( next->value );
		page_t * valuecpy; 
		memcpy(&valuecpy, &value, sizeof(page_t)); 
		next->value = valuecpy; 

	/* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair( key, value );

		/* We're at the start of the linked list in this bin. */
		if( next == hashtable->table[ bin ] ) {
			newpair->next = next;
			hashtable->table[ bin ] = newpair;
	
		/* We're at the end of the linked list in this bin. */
		} else if ( next == NULL ) {
			last->next = newpair;
	
		/* We're in the middle of the list. */
		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

/* Retrieve a key-value pair from a hash table. */
page_t *ht_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		/* Get a new frame for this. */
		// return framenumber 
		// and mark page with framenumber as invalid
		return NULL;

	} else {
		return pair->value;
	}
	
}

int main( int argc, char **argv ) {

	hashtable_t *hashtable = ht_create( 65536 );

	page_t * value1 = malloc(sizeof(page_t)); 
	value1->pagenumber = 1;
	value1->framenumber = 1;
	value1->validbit = 1;
	page_t * value2 = malloc(sizeof(page_t)); 
	value2->pagenumber = 2;
	value2->framenumber = 2;
	value2->validbit = 2;
	page_t * value3 = malloc(sizeof(page_t)); 
	value3->pagenumber = 3;
	value3->framenumber = 3;
	value3->validbit = 3;
	page_t * value4 = malloc(sizeof(page_t)); 
	value4->pagenumber = 4;
	value4->framenumber = 4;
	value4->validbit = 4;

	ht_set( hashtable, "key1", value1 );
	ht_set( hashtable, "key2", value2 );
	ht_set( hashtable, "key3", value3 );
	ht_set( hashtable, "key4", value4 );

	page_t * blah1 = ht_get( hashtable, "key1" );
	printf( "key1 %d %d %d\n", blah1->pagenumber, blah1->framenumber, blah1->validbit );
	page_t * blah2 = ht_get( hashtable, "key2" );
	printf( "key2 %d %d %d\n", blah2->pagenumber, blah2->framenumber, blah2->validbit );
	page_t * blah3 = ht_get( hashtable, "key3" );
	printf( "key3 %d %d %d\n", blah3->pagenumber, blah3->framenumber, blah3->validbit );
	page_t * blah4 = ht_get( hashtable, "key4" );
	printf( "key4 %d %d %d\n", blah4->pagenumber, blah4->framenumber, blah4->validbit );

	return 0;
}