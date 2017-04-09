#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {

	printf("pgsize: %s\n", argv[1]);
	printf("tlbentries: %s\n", argv[2]);
	printf("flag: %s\n", argv[3]);
	printf("uantum: %s\n", argv[4]);
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
	
	// for file in tracefiles {
	// 	for line in file, line<10 {
	// 		tlb.match() 
	// 	}
	// }

	return 0;
}