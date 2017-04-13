#ifndef _MEASUREMENTARRAY_H_
#define _MEASUREMENTARRAY_H_

#include <stdio.h>

typedef struct measurementarray {
	int tlbhits; 
	int tlbmisses; 
	int evictedpages; 
	int memoryresidentpagesforprocess;
} measurementarray_t;

void outputmeasurementarrays(const char* tracefiles[], int numberoftracefiles, measurementarray_t *measurementarrarr[]); 
void outputmeasurementarray(const char * tracefile, measurementarray_t * m); 

#endif
