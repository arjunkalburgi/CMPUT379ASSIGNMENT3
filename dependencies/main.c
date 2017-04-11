#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

struct rec{

	int x,y,z;

/*	tvm379
	pgsize
	tlbentries 
	{ g | p } 
	quantum 
	physpages 
	{ f | l } 
	trace1 trace2 . . .tracen
*/
};

int round_robin(int quantum, int pgsize_topass, const char* tracefiles, ...){
	//for file in tracefiles {
	//	for line in file, line<10 {
	//		tlb.match() 
	//	}
	//}
	//char* string[10];
	char* memory_extract;
	va_list arg;
	va_start(arg, tracefiles);
	while(tracefiles){
		char *string = va_arg(arg, const char *);
		FILE *ptr;
		struct rec tester;
		//printf("string: %s\n", string);

		char *result = malloc(strlen(string)+strlen(".bin")+1);//+1 for the zero-terminator
		strcpy(result, string);
		strcat(result, ".bin");
		printf("new string: %s\n", result);

		ptr = fopen(result, "rb");
		printf("made it here\n");
		if (!ptr){
			printf("Unable to open file!\n");
			return 1;
		}
		
		int x = 0;
		for(x=0; x< quantum; x++){
			fread(memory_extract, sizeof(4), 1, ptr);
			printf("made it here 2\n");
			//Pass memory extract from here? 
			//memory extract should hold one entry at a time, quantum loops through all the entries
			
			int con = atoi(memory_extract);
			int page_number = con/pgsize_topass;					

			printf("PN: %d ", page_number);
			int i;
			for (i=0; i<4; i++){
   				printf("%d ", memory_extract[i]);
			}
			printf("\n ");
			
			//printf("%s\n", memory_extract);
		}
		
		fclose(ptr);

	}
	va_end(arg);
 	return 0;
}

int main(int argc, char const *argv[])
{	
	//printf("made it here\n");
	printf("pgsize: %s\n", argv[1]);
	printf("tlbentries: %s\n", argv[2]);
	printf("flag: %s\n", argv[3]);
	printf("quantum: %s\n", argv[4]);
	printf("hyspages: %s\n", argv[5]);
	printf("flag: %s\n", argv[6]);

	int arg = 0; 
	const char *tracefiles[argc-7]; 
	for (arg = 7; arg<argc; arg++) {
		tracefiles[arg-7] = argv[arg]; 
	}
	int i = 0; 
	for (i=0; i<argc-7; i++) {
		printf("trace: %s\n", tracefiles[i]);
	}

	//quantum = atoi(argv[4]);
	//if (argc < 9) {
	//	printf("Please call like so: 'tvm379 pgsize tlbentries { g | p } quantum physpages { f | l } trace1 trace2 . . .tracen'\n");
	//	return 0; 
	//}
	//int quantum = 100;
	//strcpy(start, argv[5]); 
	//quantum = atoi(start);
	int pgsize = atoi(argv[1]);
	//double pgsize_topass = log10(pgsize)/log10(2);
	int quantum = atoi(argv[4]);

	round_robin(quantum, pgsize,"test", "heapsort-trace", "quicksort-trace");

	return 0;
}

