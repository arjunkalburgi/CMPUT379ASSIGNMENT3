#include <stdio.h>
#include <stdlib.h>
#include "measurementarray.h"

typedef struct node {
	int pagenumber;
	int framenumber;
	int validbit;
    struct node * prev;
    struct node * next;
} node_t;

typedef struct tlb {
    struct node * head; 
    struct node * end; 
    int length;
    int capacity; 
} tlb_t;

tlb_t * make_tlb(int cap);
void print_tlb_info(tlb_t * tlb);
void print_node_info(node_t * node);
void print_list(tlb_t * t);
void tlb_flush(tlb_t * t);

// with pagetable
node_t * tlb_get(tlb_t * t, int val, measurementarray_t *m);
node_t * tlb_insert(tlb_t * t, int val);

// without pagetable
int tlb_put(tlb_t * t, int val);
node_t * tlb_match(tlb_t * t, int val);