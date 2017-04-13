#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"


//./tvm379 16 16 gp 10 physpages fl heapsort-trace quicksort-trace heapsort quicksort

/*	tvm379
	pgsize
	tlbentries 
	{ g | p } 
	quantum 
	physpages 
	{ f | l } 
	trace1 trace2 . . .tracen
*/

int round_robin(int quantum, int pgsize_topass, const char* tracefiles[], int numberoftracefiles) {

	//char* memory_extract = malloc(quantum * sizeof(4));
	//va_list arg;
	//va_start(arg, tracefiles);

	// make measurementarray array
	measurementarray_t * measurementarrarr[numberoftracefiles];

	int index = 0;
	for(index = 0; index < numberoftracefiles; index++){
		
		FILE *ptr;

		char *result = malloc(strlen(tracefiles[index])+strlen(".bin")+1);//+1 for the zero-terminator
		strcpy(result, tracefiles[index]);
		strcat(result, ".bin");
		printf("new string: %s\n", result);

		ptr = fopen(result, "rb");
		if (!ptr){
			printf("Unable to open file!\n");
			return 1;
		}
		
		int x = 0;
		for(x=0; x< quantum; x++){
			uint32_t j = 0;
			fread(&j, sizeof(4), 1, ptr);
			
			// printf("the memory_extract: %u\n", j);
			uint32_t page_number = j/pgsize_topass;	//this is the page number!!
			printf("PN: %u\n", page_number);

			printf("the FN from tlb get: %d\n",(tlb_get(tlb, page_number, measurementarrarr[index]))->data->framenumber);
		}
		
		fclose(ptr);

		// if tlb is local
		if (localtlbflag == "l") {
			tlb_flush(tlb);			
		}
	}

	// output
	outputmeasurementarrays(tracefiles, numberoftracefiles, measurementarrarr); 
 	return 0;
}

int main(int argc, char const *argv[]) {	
	/*printf("pgsize: %s\n", argv[1]);
	printf("tlbentries: %s\n", argv[2]);
	printf("flag: %s\n", argv[3]);
	printf("quantum: %s\n", argv[4]);
	printf("physpages: %s\n", argv[5]);
	printf("ffl flag: %s\n", argv[6]);*/

	int numberoftracefiles = argc-7; 

	int arg = 0; 
	const char *tracefiles[numberoftracefiles]; 
	for (arg = 7; arg<argc; arg++) {
		tracefiles[arg-7] = argv[arg]; 
	}
	/*int i = 0; 
	for (i=0; i<numberoftracefiles; i++) {
		printf("trace: %s\n", tracefiles[i]);
	}*/

	// make fl
	if (argv[6] == "l") {
		frameslist = make_ffl(atoi(argv[5]), "l");
	} else {
		frameslist = make_ffl(atoi(argv[5]), "f");
	}

	// make pgtbl
	pgtable = ht_create(65536, frameslist);

	// make tlb
	tlb = make_tlb(atoi(argv[2]), pgtable, frameslist); 
	
	// set all data structures
	frameslist->hash = pgtable; 
	frameslist->tlb = tlb; 
	pgtable->tlb = tlb; 
	pgtable->frameslist = frameslist;
	tlb->hash = pgtable;
	tlb->frameslist = frameslist;

	// run round robin
	int pgsize = atoi(argv[1]);//this is the power of 2 still
	double second;//helper
	int pgsize_topass;//one we pass to round robin
	second = log10(2);
	pgsize_topass = (int)(log10(pgsize)/second);//calculate the val we pass to rr
	int quantum = atoi(argv[4]);//#entries we take, also pass to rr
	if (argv[3] == "p") {
		localtlbflag = "l";
	} else {
		localtlbflag = "g";
	}
	// printf("pgsize_topass: %d\n", (int)pgsize_topass);
	round_robin(quantum, pgsize_topass, tracefiles, numberoftracefiles);

	return 0;
}

