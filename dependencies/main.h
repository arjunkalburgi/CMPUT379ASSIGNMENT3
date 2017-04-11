
#include "tlb.h"

tlb_t *tlb; //-> pagetable -> freeframes list 
int round_robin(int quantum, int pgsize_topass, const char* tracefiles, ...);
