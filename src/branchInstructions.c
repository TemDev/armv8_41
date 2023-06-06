#include <stdio.h>
#include <math.h>
#include "branchInstructions.h"

#define BIT31302826252019181716 3592355840 // == 2 ^ 31 + 2 ^ 30 + 2 ^ 28 + 2 ^ 26 + 2 ^ 25 + 2 ^ 20 + 2 ^ 19 + 2 ^ 18 + 2 ^ 17 + 2 ^ 16
#define NOTBIT98765 4294966303 // == bitwise_not(2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5)
#define BIT98765 992 // == 2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5
#define BIT31302928272625244 4278190096 // == 2 ^ 31 + 2 ^ 30 + 2 ^ 29 + 2 ^ 28 + 2 ^ 27 + 2 ^ 26 + 2 ^ 25 + 2 ^ 24 + 2 ^ 4
#define BIT302826 1409286144 // == 2 ^ 30 + 2 ^ 28 + 2 ^ 26
#define BIT26MASK 67108863 // == 2 ^ 26 - 1
#define BIT19MASK 524287// == 2 ^ 19 - 1
#define BIT4MASK 15 // == 2 ^ 4 - 1

static void unconditional(struct CompState* state, long simm26) {
    state->PC += simm26 * 4;
    printf("STATE OF PC: %ld\n", state->PC);
};

static void registerBranch(struct CompState* state, char Rn) {
    if (Rn != 31) {
        state->PC = state->Regs[Rn];
    };
};

static void conditional(struct CompState* state, long offset, char cond) {
    	bool condRes = false;
	printf("Cond %d\n",cond);
	switch (cond) {
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
		printf("Jump\n");
		state ->PC += offset;
	} else {
		state ->PC +=4;
	}

};

void branch(struct CompState* state, int instruction) {
    if ((instruction >> 26) == 5) {
      printf("UNCONDITIONAL\n");
        long simm26 = instruction & BIT26MASK;
	if ((simm26 >> 25) == 1) {
	  simm26 = simm26 | (( (long) -1) << (long) 26);
	}
        unconditional(state, simm26);
    } else if (((instruction>>31) & 1) == 1) {
        char Rn = (char) (instruction >> 5) & 31;
        registerBranch(state, Rn);
    } else if (((instruction >> 30) & 1) == 1) {
        long simm19 = (instruction >> 5) & BIT19MASK;
	printf("simm9 before %lx \n", simm19);

	if ((simm19 >> 18) == 1) {
		simm19 = simm19 | (((long) -1) << 19);	
	}
	 printf("simm9 before %lx \n", simm19);

        char cond = (char) instruction & BIT4MASK;
        conditional(state, simm19 * 4, cond);
    };
};

static void mainBranch() {
    
};
