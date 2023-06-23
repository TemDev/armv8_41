// Main file that combines everything in each file to execute the binary instruction
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rasberryPi.h"
#include "bitwise.h"
#include "immediateInstructionProcessing.h"
#include "RegisterInstructionProcessing.h"
#include "singleMTransfer.h"
#include "branchInstructions.h"

#define HLT 0x8a000000
#define NOP 0xd503201f
#define E_FLAG 0xfffff
#define WRITE_REG 0xfff


// Outputs onto file the states in the CompState structure which includes the states of all the registers as well as the flags in PSTATE
static void printCompState(FILE *fp, CompState* state) {
    fprintf(fp, "Registers:\n");
    for (int i = 0; i<31; i++) {
        fprintf(fp, "X%02d   = %.16lx\n", i, state -> Regs[i]);
    }
    fprintf(fp, "PC    = %.16lx\n",  state->PC);
    char st[4];
    if (state -> PSTATE.N) {
	st[0] = 'N';
    } else {
	st[0] = '-';
    }
    if (state -> PSTATE.Z) {
	st[1] = 'Z';
    } else {
	st[1] = '-';
    }
    if (state -> PSTATE.C) {
	st[2] = 'C';
    } else {
	st[2] = '-';
    }
    if (state -> PSTATE.V) {
	st[3] = 'V';
    } else {
	st[3] = '-';
    }
    fprintf(fp, "PSTATE : %s\n", st);
}

// Sets the initial states in the CompState structure. All registers to 0, all flags other than Z to false, Z is true
static void initial(CompState* statep) {
    statep->ZR = 0;
    statep->PC  = 0;
    statep->PSTATE.N = false;
    statep->PSTATE.Z = true;
    statep->PSTATE.C = false;
    statep->PSTATE.V = false;
    statep->SP = 0;
	
    for (int i = 0; i < NUM_GEN_REG; i++) {
	statep->Regs[i] = 0;
    }
}

int processInstruction(CompState* state, int32_t instruction, char* memory) {
	char Op0 = (instruction >> 25) & 15;
	printf("Instruction is %.8x \n", instruction);
	if (instruction == HLT) {
        printf("Program halting\n");
	    return -1;
	} else if (instruction == NOP) {
        printf("NOP instruction executed\n");
	    state -> PC += 4;
	}else if (Op0 == 8 || Op0 == 9) {
        printf("Immediate processing\n");	
	    determineTypeImmediate(state, instruction);
	   state -> PC += 4;
	} else if (Op0 == 5 || Op0 == 13) {
        printf("Register processing\n");
	    determineTypeRegister(state, instruction);
	    state -> PC += 4;
	} else if (Op0 == 4 || Op0 == 6 || Op0 == 12 || Op0 == 14) {
	    // Single Data Transfer determineType to go here
        printf("Single Data Transfer Instruction\n");
	    unsignedImmOffset(state, instruction, memory);
	    state -> PC += 4;
        } else if (Op0 == 10 || Op0 == 11) {
        printf("Branch\n");
	    branch(state, instruction);
	} else {
	    state -> PC += 4;
	}
    return 0;
}

// Main function of executing binary file. Takes in three command line arguments, the first one is the executable file and so is irrelevant, the second one is the binary file to execute and the third is the path of the file of output.
int main(int argc, char** argv) {
    // checks whether there is exactly one output
    /*if( argc != 3 ) {
	fprintf( stderr, "Usage: cat filename!\n" );
	exit(1);
    }*/
    FILE *fp = fopen(argv[1], "rb");
    char *memory = (char*) malloc(MEM_SIZE * sizeof(char));
    int count = 0;
    while (!feof(fp) && count < MEM_SIZE) {
	fread(&memory[count], sizeof(char), 1, fp);
	count++;
    }
    fclose(fp);
    
    CompState state;
    initial(&state);
    int32_t instruction;
    char c;
    bool b = false;
    //accessMemory(&instruction, state -> PC, memory, 4);
    do { // it is supposed to be != but in this case is in infinite loop
	    accessMemory(&instruction, memory,state.PC, 4, 'w');
        
        
        c = getchar();
        while (c == 'p' || c == 'm' || c == 'q' || c =='\n') {
            if (c == 'p') {
              printCompState(stdout, &state);
              c = getchar();

            } else if (c == 'm') {
              for (int i = 0; i < MEM_SIZE; i += 4){
	                int32_t location;
	                accessMemory(&location, memory, i, 4, 'w');
	              if (location != 0) {      
	                 printf("0x%.8x : %.8x\n", i, location);
	             }
                 if (i == 0x1000) {
                    printf("start of the on buffer ");
                    printf("0x%.8x : %.8x\n", i, location);
                 }
            }
            c = getchar();


        } else if (c == 'q') {
            printf("exiting forcefully\n");
            c == '\0';
            b = true;
            break;
        } else if (c == '\n') {
            c = getchar();

        }
        

          }
          if (b) {
            b = false;
            break;
          }
          int res = processInstruction(&state, instruction, memory);
          printf("PC is %x\n", state.PC);
        if (res == -1) {
            printf("exited normally after HLT instruction\n");
            break;
        }
        
        int32_t readReg;
        // accessMemory(&readReg, memory, E_FLAG, 4, 'w');
        // if (readReg == 0x5555) {
        //     printf("the read-register is set\n");
        // }


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

    FILE *file = fopen(argv[2], "wb");
    //printf("size of the memory is %d\n", count);
    printCompState(file, &state);
    fprintf(fp,"Non-Zero Memory:\n");
    for (int i = 0; i < MEM_SIZE; i += 4){
	int32_t location;
	accessMemory(&location,memory, i, 4, 'w');
	if (location != 0) {      
	   fprintf(file,"0x%.8x : %.8x\n", i, location);
	}
    }

    //closes opened files and frees the memory
    free(memory);

    fclose(file);

    return EXIT_SUCCESS;
}
