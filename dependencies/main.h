
#include "tlb.h"
#include "measurementarray.h"


tlb_t *tlb; //-> pagetable -> freeframes list 
struct measurementarray_t ** measurementarr;

int round_robin(int quantum, int pgsize_topass, const char* tracefiles, ...);


