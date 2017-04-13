#ifndef _FREEFRAMESLIST_H_
#define _FREEFRAMESLIST_H_

#include "tlb.h"
#include "pgtbl.h"

typedef struct frame {
	int framenumber; 
    struct frame * prev;
    struct frame * next;
} frame_t;

typedef struct ffl_s {
    struct frame * head; 
    struct frame * end; 
    int size;
    int capacity;
    int type; //lru=1, fifo=0
    struct hashtable_s * hash; 
    struct tlb *tlb;
} ffl_t;

ffl_t * make_ffl(int cap, int flag);
int ffl_get(ffl_t * l);
void ffl_addframeToUsedList(ffl_t * l, int framenumber);
void ffl_update(ffl_t * l, int framenumber); 

#endif
