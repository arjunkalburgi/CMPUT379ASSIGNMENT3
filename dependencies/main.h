#ifndef _MAIN_H_
#define _MAIN_H_

#include "tlb.h"
#include "pgtbl.h"
#include "measurementarray.h"

hashtable_t *hashtable;
tlb_t *tlb; //-> pagetable -> freeframes list 

measurementarray_t * measurementarrarr;

int round_robin(int quantum, int pgsize_topass, const char* tracefiles[], int numberoftracefiles, measurementarray_t *measurementarrarr);



#endif
