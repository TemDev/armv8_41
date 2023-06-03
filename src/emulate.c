#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "emulate.h"
#include "bitwise.h"
#include "immediateInstructionProcessing.h"
#include "RegisterInstructionProcessing.h"
#include "singleMTransfer.h"
#include "branchInstructions.h"


#define HLT 0x8a000000
     
void printCompState(FILE *fp, struct CompState* state) {
	fprintf(fp, "%5s : %.16x \n", "PC",  state->PC);
	fprintf(fp, "%5s : %.16x \n", "ZR", state->ZR);
	fprintf(fp, "%5s : %.16x \n", "PSTATE", state->PSTATE);
	fprintf(fp, "%5s : %.16x \n", "SP",state->SP);
	for (int i = 0; i<NUM_GEN_REG; i++) {
		fprintf(fp, "X%d: %.16x \n", i, state -> Regs[i]);
	}
}

void initial(struct CompState* statep) {
	statep->ZR = 0;
	statep->PC  = 0;
	statep->PSTATE.N = 0;
	statep->PSTATE.Z = 0;
	statep->PSTATE.C = 0;
	statep->PSTATE.V = 0;
	statep->SP = 0;
	for (int i = 0; i < NUM_GEN_REG; i++) {
		statep->Regs[i] = 0;
	}
}


int main(int argc, char** argv) {
	// checks whether there is exactly one output
	if( argc != 3 ) {
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
	fclose(fp);
	struct CompState state;
	initial(&state);
	int instruction;
	do { // it is supposed to be != but in this case is in infinite loop
		char Op0 = (instruction >> 25) & 15;
		accessMemory(&instruction, state.PC, memory, 4);
		printf("instruction is %d \n", instruction);
		if (Op0 == 8 || Op0 == 9) {
			printf("Immediate Arithmetic \n");	
		    determineTypeImmediate(&state, instruction);
		    state.PC += 4;
		} else if (Op0 == 5 || Op0 == 13) {
			printf("register Arithmetic \n");
		    determineTypeRegister(&state, instruction);
		    state.PC += 4;
		} else if (Op0 == 4 || Op0 == 6 || Op0 == 12 || Op0 == 14) {
			// Single Data Transfer determineType to go here
			printf("Memory \n");
		    	unsignedImmOffset(&state, instruction, memory);
			state.PC += 4;
		} else if (Op0 == 10 || Op0 == 11) {
			printf("branch \n");
		    branch(&state, instruction);
		} else {
			printf("other \n");
		    state.PC += 4;
		}
		
	} while (instruction != HLT );
	

	
	/*state.Regs[1] = 2;
	state.Regs[2] = 20;
	long x = 0x6789678967896789;
	printf("x:0x%lx", x);
	memcpy(&memory[20], &x, 8);

	printf("Register 1 is %d\n", state.Regs[1]);
	printf("Register 2 is %d\n", state.Regs[2]);
	unsigned int input = B("1 1 1110 00 01 100001001010 00001 01010");
	printf("%d\n", input);
	unsignedImmOffset(&state, input, memory);
	x = 0x12345678;
	memcpy(&memory[100], &x, 8); 
	*/
	
	
	FILE *file = fopen(argv[2], "w");
	printf("size of the memory is %d\n", count);
	printCompState(file, &state);
	for (int i = 0; i < MEM_SIZE; i += 4){
		int location;
	        accessMemory(&location, i, memory, 4);
		if (location != 0) {      
			fprintf(file,"0x%.8x : 0x%.8x\n", i, location);
		}
	}

	//closes opened files and frees the memory
	free(memory);
 	fclose(file);

	return EXIT_SUCCESS;


}

