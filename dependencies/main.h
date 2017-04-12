#ifndef _MAIN_H_
#define _MAIN_H_

#include "tlb.h"
#include "pgtbl.h"
#include "freeframeslist.h"
#include "measurementarray.h"

ffl_t * frameslist; 
hashtable_t *pgtable;
tlb_t *tlb; //-> pagetable -> freeframes list 
struct measurementarray_t ** measurementarr;

int round_robin(int quantum, int pgsize_topass, const char* tracefiles, ...);

#endif
