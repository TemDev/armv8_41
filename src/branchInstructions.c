// File that determines the specific type of branch instruction and then executes the branch by changing PC value in CompState structure.

#include <stdio.h>
#include <math.h>
#include "branchInstructions.h"

#define BIT26MASK 67108863 // == 2 ^ 26 - 1
#define BIT19MASK 524287// == 2 ^ 19 - 1
#define BIT4MASK 15 // == 2 ^ 4 - 1
#define NUMREG 31 // Number of registers

// Function used for unconditional branches
static void unconditional(CompState* state, int64_t simm26) {
    state->PC += simm26 * 4;
};

// Function used for register branching
static void registerBranch(CompState* state, unsigned char Rn) {
    if (Rn != NUMREG) {
        state->PC = state->Regs[Rn];
    };
};

// Function used for conditional branching
static void conditional(CompState* state, int64_t offset, char cond) {
    bool condRes = false;
    printf("Cond %d\n",cond);
    switch (cond) {
    // Cases with values as defined by the spec
    case 0:
        condRes = (state->PSTATE.Z);
        break;
    case 1:
        condRes = !(state->PSTATE.Z);
        break;
    case 10:
        condRes = !(state->PSTATE.N ^ state->PSTATE.V);
        break;
    case 11:
        condRes = (state->PSTATE.N ^ state->PSTATE.V);
        break;
    case 12:
        condRes = (!(state->PSTATE.Z) && !(state->PSTATE.N ^ state->PSTATE.V));
        break;
    case 13:
        condRes = (!(!(state->PSTATE.Z) && !(state->PSTATE.N ^ state->PSTATE.V)));
        break;
    case 14:
        condRes =(state->PSTATE.N) || (state->PSTATE.Z) || (state->PSTATE.C) || (state-> PSTATE.V);
    	break;
    default:
	condRes = false;
	break;
    };
    if (condRes) {
	state ->PC += offset;
    } else {
	state ->PC += 4; // 4 as each instruction takes 4 bytes
    }
};

// Function that determines what type of branch is being requested
void branch(CompState* state, int32_t instruction) {
    if ((instruction >> 26) == 5) { // Masking to determine if matches
        printf("UNCONDITIONAL\n");
        int64_t simm26 = instruction & BIT26MASK;
	// Looks at the 25th bit of simm26 in order to determine whether sign extension is needed
	if ((simm26 >> 25) == 1) { 
	    simm26 = simm26 | (( (int64_t) -1) << (int64_t) 26);
	}
        unconditional(state, simm26);
    } else if (((instruction>>31) & 1) == 1) { // Masking to determine if matches
        unsigned char Rn = (unsigned char) (instruction >> 5) & 31; // Determines the Rn register
        registerBranch(state, Rn);
    } else if (((instruction >> 30) & 1) == 1) { // Masking to determine if matches
        int64_t simm19 = (instruction >> 5) & BIT19MASK;
	// Looks at the 18th bit of simm19 in order to determine whether sign extension is needed
	if ((simm19 >> 18) == 1) {
	    simm19 = simm19 | (((int64_t) -1) << 19);	
	}

        char cond = (char) instruction & BIT4MASK;
        conditional(state, simm19 * 4, cond);
    };
};
