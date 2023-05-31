#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "singleMTransfer.h"
#define MEM_SIZE 16394
#define HLT 0x8a000000

int main(int argc, char** argv) {
	// checks whether there is exactly one output
	if( argc != 2 ) {
	        fprintf( stderr, "Usage: cat filename!\n" );
	        exit(1);
	}
	FILE *fp = fopen(argv[1], "rb");
	char *memory = (char*) malloc(MEM_SIZE * sizeof(char));
	int count = 0;
	while (!feof(fp) && count < MEM_SIZE) {
		fread(&memory[count], sizeof(char), 1, fp);
		count++;
	}
	
	struct CompState state;
	
	int instruction = memory[state.PC];
	while (instruction == HLT ) { // it is supposed to be != but in this case is in infinite loop
		// process this instruction 
		// update the CompState
		// and memory corresponding to the
		// next insturction
		// get the next instruction back 
		// to inst value
		char Op0 = (instruction >> 25) & 15;
		if (Op0 == 8 || Op0 == 9) {
		    
		} else if (Op0 == 5 || Op0 == 13) {
		    
		} else if (Op0 == 4 || Op0 == 6 || Op0 == 12 || Op0 == 14) {
		    
		} else if (Op0 == 10 || Op0 == 11) {
		    
		};
		state.PC += 4;
		
	};
	
	state.Regs[1] = 2;
	state.Regs[2] = 3;
	unsignedImmOffset(&state, 0b11111000010000000001110001000001,memory);
	
	
	FILE *file = fopen("hello.out", "w");
	printf("size of the memory is %d\n", count);

	for (int i = 0; i < MEM_SIZE; i += 4){
		int location = accessMemory(i, memory, 4);
		if (location != 0) {      
			fprintf(file,"0x%.8x : 0x%.8x\n",
				       	i, location);
		}
	}

	//closes opened files and frees the memory
	free(memory);
	fclose(fp);
 	fclose(file);

	return EXIT_SUCCESS;
};