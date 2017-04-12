#ifndef _FREEFRAMESLIST_H_
#define _FREEFRAMESLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include "page.h"

typedef struct frame {
	int framenumber; 
    struct node * prev;
    struct node * next;
} frame_t;

typedef struct ffl {
    struct node * head; 
    struct node * end; 
    int size;
    int capacity;
    int type; //lru=0, fifo=1
} ffl_t;

ffl_t * make_ffl(int cap);
int ffl_get(ffl_t * l);
void ffl_addframeToUsedList(ffl_t * l, int framenumber);

#endif
