#ifndef _TLB_H_
#define _TLB_H_

#include <stdio.h>
#include <stdlib.h>
#include "pgtbl.h"
#include "freeframeslist.h"
#include "page.h"
#include "measurementarray.h"

typedef struct node {
	struct page * data; 
    struct node * prev;
    struct node * next;
} node_t;

typedef struct tlb {
    struct node * head; 
    struct node * end; 
    int length;
    int capacity; 
    struct hashtable_s * hash; 
    struct ffl_s * frameslist; 
} tlb_t;


tlb_t * make_tlb(int cap, struct hashtable_s * h, struct ffl_s * f);
void print_tlb_info(tlb_t * tlb);
void print_node_info(node_t * node);
void print_list(tlb_t * t);
void tlb_flush(tlb_t * t);

// with pagetable
node_t * tlb_get(tlb_t * t, int pagenumber, measurementarray_t *m);
node_t * tlb_insert(tlb_t * t, int pagenumber, measurementarray_t *m);
node_t * tlb_match(tlb_t * t, int pagenumber);
void tlb_framematch(tlb_t * t, int framenumber);

// without pagetable
//int tlb_put(tlb_t * t, int val);

#endif
