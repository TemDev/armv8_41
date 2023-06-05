#include <stdio.h>
#include <math.h>
#include "emulate.h"
#include "immediateInstructionProcessing.h"
#include "bitwise.h"

#define BIT24 16777216 // == 2 ^ 24, MASK
#define BIT2523 41943040 // == 2 ^ 25 + 2 ^ 23, MASK
#define BITsf 2147483648 // == 2 ^ 31, MASK
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0, MASK
#define BIT32MASK 4294967295 // == 2 ^ 32 - 1, MASK
#define REGISTER31 31
#define BIT6432 0b1111111111111111111111111111111100000000000000000000000000000000
#define BIT31 2147483648 // == 2 ^ 31, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK

void add(struct CompState* state, int instruction, char Rn, long Op) {
  printf("ADD %d \n", Op);
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
      printf("64-bit \n");
        long result;
        if (Rn == REGISTER31) {
            result = state->SP + Op;
        } else {
	    result = state->Regs[Rn] + ((long) Op);
	    printf("%x", state->Regs[Rd]);
        };

	if (Rd == REGISTER31) {
	  state->SP = result;
	} else {
	  state->Regs[Rd] = result;
	};
        
    } else {
        int result;
        if (Rn == REGISTER31) {
	        if (state->SP & BIT31) {
                result = state->SP | BIT6432;
	        } else {
                result = state->SP & (BIT32 - 1);
	        };
            result += Op;
        } else {
	  printf("32-bit");
            if (state->Regs[Rn] & BIT31) {
                result = state->Regs[Rn] | BIT6432;
            } else {
                result = state->Regs[Rn] & (BIT32 - 1);
            };
	    printf("Result after: %ld\n", state->Regs[Rn]);
	    printf("Result before: %ld\n", result);
            result += Op;
	    printf("Result after: %ld\n", result);
        };

	if (Rd == REGISTER31) {
	  state->SP = result;
            state->SP = state->SP & (BIT32 - 1);
	} else {
	  state->Regs[Rd] = result;
	  state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
	};
    };
};

void adds(struct CompState* state, int instruction, char Rn, long Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
      printf("64 bit version\n");
        long result;
        if (Rn == REGISTER31) {
            result = state->ZR + Op;
            state->PSTATE.V = 0;
            state->PSTATE.C = 0;
        } else {
            result = state->Regs[Rn] + Op;
            state->PSTATE.V = (state->Regs[Rn] > 0 && Op > 0 && result < 0) || (state->Regs[Rn] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rn] < 0 && Op < 0) || (state->Regs[Rn] < 0 && Op > 0 && result >= 0) || (state->Regs[Rn] > 0 && Op < 0 && result >= 0);
        };
	if (!(Rd == REGISTER31)) {
	  state->Regs[Rd] = result;
	};
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
        
    } else {
      printf("32 bit version\n");
        int result;
        if (Rn == REGISTER31) {
                result = state->ZR;
	        result += Op;
            state->PSTATE.V = 0;
            state->PSTATE.C = 0;
        } else {
	  printf("state regs mask%d\n", state->Regs[Rd] & BIT31);
	        if (state->Regs[Rn] & BIT31) {
	            result = state->Regs[Rn] | BIT6432;
	        } else {
	            result = state->Regs[Rn] & (BIT32 - 1);
	        };
	        result += Op;
            state->PSTATE.V = (state->Regs[Rn] > 0 && Op > 0 && result < 0) || (state->Regs[Rn] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rn] < 0 && Op < 0) || (state->Regs[Rn] < 0 && Op > 0 && result >= 0) || (state->Regs[Rn] > 0 && Op < 0 && result >= 0);
        };

	if (!(Rd == REGISTER31)) {
	  state->Regs[Rd] = result;
	  state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
	};
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
    };
};

static void arithmeticImmediate(struct CompState* state, int instruction) {
    printf("Arithmetic");
    const int sh = 4194304;
    const int imm12 = 4095; // 2 ^ 12 - 1
    const short rn = 992;
    long Op = ((instruction >> 10) & imm12) << (12 * ((instruction >> 22) & 1));
    printf("%d\n", Op);
    char Rn = (rn & instruction) >> 5;
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        adds(state, instruction, Rn, (-Op));
        break;
        case 2:
        add(state, instruction, Rn, (-Op));
        break;
        case 1:
        adds(state, instruction, Rn, Op);
        break;
        default:
        add(state, instruction, Rn, Op);
    }
};

static void movn(struct CompState* state, int instruction, long Op) {
    char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
      printf("%ld", ~Op);
        state->Regs[Rd] = ~Op;
    } else { // 32 bit mode.
        state->Regs[Rd] = ~Op;
	state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

static void movz(struct CompState* state, int instruction, long Op) {
    char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
        state->Regs[Rd] = Op;
    } else {
        state->Regs[Rd] = Op;
	state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

static void movk(struct CompState* state, int instruction, long imm16, int hw) {
    printf("movk");
    char Rd = BITrd & instruction;
    long MASK2 = (long)  ~((long) 65535 << (((long) hw) * ((long) 16)));
    printf("MASK2 %lx \n", MASK2);
    long MASK = imm16 << (hw * 16);
    printf("MASK1 %lx \n", MASK);
    printf("Before %lx \n", state->Regs[Rd]);
    state->Regs[Rd] = (state->Regs[Rd] & MASK2) + MASK;
    printf("After %lx \n", state->Regs[Rd]);
    if (!(BITsf & instruction)) {
      state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

static void wideMove(struct CompState* state, int instruction) {
  int hw = (instruction >> 21) & 3;
  int opc = (instruction >> 29) & 3;
    long imm16MASK = 65535; // 2 ^ 16 - 1
    long imm16 = lsr_32(instruction, 5) & imm16MASK;
    long Op = imm16 <<( hw * 16);
    printf("shifted %d\n", (instruction >> 29));
    printf("opc %d\n", opc);
    if (opc == 3) {
        movk(state, instruction, imm16, hw);
    } else if (opc == 2) {
        movz(state, instruction, Op);
    } else {
        movn(state, instruction, Op);
    };
    
};

// Determines type of immediate instruction, arithmetic or wideMove.
void determineTypeImmediate(struct CompState* state, int instruction) {
  printf("determineType\n");
    if (instruction & (BIT24)) {
        arithmeticImmediate(state, instruction);
    } else if ((instruction >> 23) & 5 == 5) {
      printf("wideMOve\n");
        wideMove(state, instruction);
    };
};

static int main(void) {
    
};
