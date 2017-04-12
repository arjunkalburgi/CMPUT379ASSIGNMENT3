typedef struct measurementarray {
	int tlbhits; 
	int tlbmisses; 
	int evictedpages; 
	int memoryresidentpagesforprocess;
} measurementarray_t;

void outputmeasurementarray(char[] tracefile, measurementarray_t * m); 
void recordhit(measurementarray_t *m); 