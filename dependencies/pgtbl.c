// implementation modified from https://gist.github.com/tonious/1377667
#include "pgtbl.h"

/* Create a new hashtable. */
hashtable_t *ht_create( int size, ffl_t * f) {

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
	if( ( hashtable->frametable = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->frametable[i] = NULL;
	}

	hashtable->size = size;
    hashtable->frameslist = f; 

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
		// PAGETABLE MISS
		page_t * pg = malloc(sizeof(page_t)); 
		pg->pagenumber = atoi(key);
		pg->framenumber = ffl_get(hashtable->frameslist); /* Get a new frame for this page */
		pg->validbit = 1;
		ht_set(hashtable, key, pg); // add to the hashtable
		return pg; 
	} else {
		// PAGETABLE HIT UPDATE FFL (if lru)
		ffl_update(hashtable->frameslist, pair->value->framenumber);
		return pair->value;
	}
	
}

void ht_framematch(hashtable_t *h, int framenumber) {
	
	char * key; // = itoa(framenumber); // framenumber to char 
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( h, key );

	/* Step through the bin, looking for our value. */
	pair = h->frametable[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	page_t * pg = pair->value; 

	// set pg's bit to invalid

	// call tlb's framematch with page

}

// void ht_frameset, make the thing too 
