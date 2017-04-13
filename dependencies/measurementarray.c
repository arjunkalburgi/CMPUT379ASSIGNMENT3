#include "measurementarray.h"

void outputmeasurementarrays(const char* tracefiles[], int numberoftracefiles, measurementarray_t *measurementarrarr[]) {

	int index = 0;
	for(index = 0; index < numberoftracefiles; index++) {
		outputmeasurementarray(tracefiles[index], measurementarrarr[index]); 
	}

}

void outputmeasurementarray(const char * tracefile, measurementarray_t * m) {
	printf("%s %d %d %d \n", tracefile, m->tlbhits, m->tlbmisses, m->evictedpages);
}
