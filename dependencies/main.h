#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include "measurementarray.h"
#include "tlb.h"
// #include "pgtbl.h"
// #include "freeframeslist.h"

ffl_t * frameslist; 
hashtable_t *pgtable;
tlb_t *tlb; //-> pagetable -> freeframes list 
struct measurementarray_t ** measurementarr;

int round_robin(int quantum, int pgsize_topass, const char* tracefiles[], int numberoftracefiles, measurementarray_t *measurementarrarr[]);

#endif
