#include <stdio.h>
#include <math.h>
#include "emulate.h"
#include "immediateInstructionProcessing.h"
#include "bitwise.h"#define BIT32MASK 4294967295 // == 2 ^ 32 - 1, MASK

#define BIT24 16777216 // == 2 ^ 24, MASK
#define BITsf 2147483648 // == 2 ^ 31, MASK
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0, MASK
#define REGISTER31 31
#define BIT6432 0b1111111111111111111111111111111100000000000000000000000000000000
#define BIT31 2147483648 // == 2 ^ 31, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK

// Function used to add contentsof register Rn and an operand Op and load into register. Does not set flags.
void add(struct CompState* state, int instruction, char Rn, long Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == REGISTER31) {
            result = state->SP + Op;
        } else {
	    result = state->Regs[Rn] + ((long) Op);
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
            if (state->Regs[Rn] & BIT31) {
                result = state->Regs[Rn] | BIT6432;
            } else {
                result = state->Regs[Rn] & (BIT32 - 1);
            };
            result += Op;
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

// Function used to add contentsof register Rn and an operand Op and load into register. Sets flags. 
void adds(struct CompState* state, int instruction, char Rn, long Op, char subOrNot) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == REGISTER31) {
            result = state->ZR + Op;
            state->PSTATE.V = false;
            state->PSTATE.C = false;
        } else {
            result = state->Regs[Rn] + Op;
            state->PSTATE.V = (state->Regs[Rn] > 0 && Op > 0 && result < 0) || (state->Regs[Rn] < 0 && Op < 0 && result > 0);
	    /*
	    long temp = ((lsr_64(state->Regs[Rn], 1) + (lsr_64(Op, 1)) + ((state->Regs[Rn] & Op) & 1))); 
            if (subOrNot ==1) {
	        state -> PSTATE.C = temp <= 0;
	    } else {
	        state ->PSTATE.C = temp < 0;
	    }*/	  
	    state->PSTATE.C = (state->Regs[Rn] < 0 && Op < 0) || (state->Regs[Rn] < 0 && Op >= 0 && result >= 0) || (state->Regs[Rn] >= 0 && Op < 0 && result >= 0) || (Op == 0 && subOrNot);
        };
	
	if (!(Rd == REGISTER31)) {
	    state->Regs[Rd] = result;
	};
	
	state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
        
    } else {
        int result;
        if (Rn == REGISTER31) {
            result = state->ZR;
	    result += Op;
            state->PSTATE.V = false;
            state->PSTATE.C = false;
        } else {
	    if (state->Regs[Rn] & BIT31) {
	        result = state->Regs[Rn] | BIT6432;
	    } else {
	        result = state->Regs[Rn] & (BIT32 - 1);
	    };
	    int res = result;
	    int opp = Op;

	    result += ((int) Op);

            state->PSTATE.V = (res > 0 && opp > 0 && result < 0) || (res < 0 && opp < 0 && result > 0);
	    /*
	    int temp = ((lsr_32(res, 1) + (lsr_32(Op, 1)) + ((state->Regs[Rn] & Op) & 1)));
            if (subOrNot ==1) {
	        state -> PSTATE.C = temp <= 0;
	    } else {
	        state ->PSTATE.C = temp < 0;
	    }*/
	    
	    state->PSTATE.C = (res < 0 && opp < 0) || (res < 0 && opp >= 0 && result >= 0) || (res >= 0 && opp < 0 && result >= 0) || ( opp == 0 && subOrNot);
	};

	if (!(Rd == REGISTER31)) {
	    state->Regs[Rd] = result;
	    state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
	};
	printf("final RES : %x\n", result);
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
    };
    
};

// Function used to handle arithmetic operations - add, adds, sub, subs
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
	adds(state, instruction, Rn, (-Op), 1);
        break;
        case 2:
        add(state, instruction, Rn, (-Op));
        break;
        case 1:
	adds(state, instruction, Rn, Op, 0);
        break;
        default:
        add(state, instruction, Rn, Op);
    }
};

// Function that handles movn operations
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

// Function that handles movz operations
static void movz(struct CompState* state, int instruction, long Op) {
    char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
        state->Regs[Rd] = Op;
    } else {
        state->Regs[Rd] = Op;
	state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

// Function that handles movk operations
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

// Function that determines what type of wideMove operation is to be performed
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

int main(void) {
    
};
