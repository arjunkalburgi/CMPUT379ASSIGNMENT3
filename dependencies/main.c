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

int round_robin(int quantum, int pgsize_topass, const char* tracefiles[], int numberoftracefiles, 	measurementarray_t *measurementarrarr[]){

	//char* memory_extract = malloc(quantum * sizeof(4));
	//va_list arg;
	//va_start(arg, tracefiles);
	int index = 0;
	for(index = 0; index < numberoftracefiles; index++){
		
		FILE *ptr;

		//printf("string: %s\n", string);

		char *result = malloc(strlen(tracefiles[index])+strlen(".bin")+1);//+1 for the zero-terminator
		strcpy(result, tracefiles[index]);
		strcat(result, ".bin");
		printf("new string: %s\n", result);

		ptr = fopen(result, "rb");
		//printf("made it here\n");
		if (!ptr){
			printf("Unable to open file!\n");
			return 1;
		}
		
		int x = 0;
		for(x=0; x< quantum; x++){
			uint32_t j = 0;
			fread(&j, sizeof(4), 1, ptr);
			//printf("made it here 2\n");
			//Pass memory extract from here? 
			//memory extract should hold one entry at a time, quantum loops through all the entries
			

			printf("the memory_extract: %u\n", j);
			uint32_t page_number = j/pgsize_topass;	//this is the page number!!				
			printf("PN: %u\n", page_number);
			// here, i think --> tlb(j, page_number);
			printf("made it b4 get\n");

			printf("the PN from tlb get: %d\n",(tlb_get(tlb, page_number, measurementarrarr[index]))->data->pagenumber);
			printf("made it out get\n");
			//tlb_get(tlb, page_number, measurementarrarr[index]);

		}
		
		fclose(ptr);
		// if local -> flush 

	}
	
 	return 0;
}

int main(int argc, char const *argv[]) {	
	//printf("made it here\n");
	printf("pgsize: %s\n", argv[1]);
	printf("tlbentries: %s\n", argv[2]);
	printf("flag: %s\n", argv[3]);
	printf("quantum: %s\n", argv[4]);
	printf("physpages: %s\n", argv[5]);
	printf("ffl flag: %s\n", argv[6]);

	int numberoftracefiles = argc-7; 

	int arg = 0; 
	const char *tracefiles[numberoftracefiles]; 
	for (arg = 7; arg<argc; arg++) {
		tracefiles[arg-7] = argv[arg]; 
	}
	int i = 0; 
	for (i=0; i<numberoftracefiles; i++) {
		printf("trace: %s\n", tracefiles[i]);
	}

	// make fl
	frameslist = make_ffl(argv[5], argv[6]);

	// make pgtbl
	pgtable = ht_create(65536, frameslist);
	printf("made it after hashtable create\n");

	// make tlb
	tlb = make_tlb(atoi(argv[2]), pgtable, frameslist); //-> pagetable -> freeframes list 
	printf("made it after make_tlb\n");

	//printf("made it here\n");
	// make measurementarray array
	measurementarray_t *measurementarrarr[numberoftracefiles]; 

	int pgsize = atoi(argv[1]);//this is the power of 2 still
	double second;//helper
	int pgsize_topass;//one we pass to round robin
	second = log10(2);
	pgsize_topass = (int)(log10(pgsize)/second);//calculate the val we pass to rr
	int quantum = atoi(argv[4]);//#entries we take, also pass to rr
	printf("pgsize_topass: %d\n", (int)pgsize_topass);
	round_robin(quantum, pgsize_topass, tracefiles, numberoftracefiles, measurementarrarr);
	return 0;
}

