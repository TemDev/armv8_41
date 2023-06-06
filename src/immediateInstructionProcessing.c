// Handles immediate instructions by determining their type and changing appropriate registers and flags.

#include <stdio.h>
#include <math.h>
#include "emulate.h"
#include "immediateInstructionProcessing.h"
#include "bitwise.h"

#define BIT32MASK 4294967295 // == 2 ^ 32 - 1, MASK
#define BIT24 16777216 // == 2 ^ 24, MASK
#define BITsf 2147483648 // == 2 ^ 31, MASK
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0, MASK
#define REGISTER31 31
#define BIT6432 0b1111111111111111111111111111111100000000000000000000000000000000
#define BIT31 2147483648 // == 2 ^ 31, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK

// Function used to add contentsof register Rn and an operand Op and load into register. Does not set flags.
void add(struct CompState* state, int instruction, unsigned char Rn, int64_t Op) {
    unsigned char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        int64_t result;
        if (Rn == REGISTER31) {
            result = state->SP + Op;
        } else {
	    result = state->Regs[Rn] + ((int64_t) Op);
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
void adds(struct CompState* state, int32_t instruction, unsigned char Rn, int64_t Op, char subOrNot) {
    unsigned char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        int64_t result;
        if (Rn == REGISTER31) {
            result = state->ZR + Op;
            state->PSTATE.V = false;
            state->PSTATE.C = false;
        } else {
            result = state->Regs[Rn] + Op;
            state->PSTATE.V = (state->Regs[Rn] > 0 && Op > 0 && result < 0) || (state->Regs[Rn] < 0 && Op < 0 && result > 0);
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
	    int32_t res = result;
	    int32_t opp = Op;

	    result += ((int32_t) Op);

            state->PSTATE.V = (res > 0 && opp > 0 && result < 0) || (res < 0 && opp < 0 && result > 0);
	    state->PSTATE.C = (res < 0 && opp < 0) || (res < 0 && opp >= 0 && result >= 0) || (res >= 0 && opp < 0 && result >= 0) || (opp == 0 && subOrNot);
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
static void arithmeticImmediate(struct CompState* state, int32_t instruction) {
    printf("Arithmetic");
    const int32_t imm12 = 4095; // 2 ^ 12 - 1
    const short rn = 992;
    int64_t Op = ((instruction >> 10) & imm12) << (12 * ((instruction >> 22) & 1));
    unsigned char Rn = (rn & instruction) >> 5;
    unsigned char opc = (instruction >> 29) & 3;
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
static void movn(struct CompState* state, int32_t instruction, int64_t Op) {
    unsigned char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
      printf("%ld", ~Op);
        state->Regs[Rd] = ~Op;
    } else { // 32 bit mode.
        state->Regs[Rd] = ~Op;
	state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

// Function that handles movz operations
static void movz(struct CompState* state, int32_t instruction, int64_t Op) {
    unsigned char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
        state->Regs[Rd] = Op;
    } else {
        state->Regs[Rd] = Op;
	state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

// Function that handles movk operations
static void movk(struct CompState* state, int32_t instruction, int64_t imm16, int32_t hw) {
    printf("movk");
    unsigned char Rd = BITrd & instruction;
    int64_t MASK2 = (int64_t)  ~((int64_t) 65535 << (((int64_t) hw) * ((int64_t) 16)));
    printf("MASK2 %lx \n", MASK2);
    int64_t MASK = imm16 << (hw * 16);
    printf("MASK1 %lx \n", MASK);
    printf("Before %lx \n", state->Regs[Rd]);
    state->Regs[Rd] = (state->Regs[Rd] & MASK2) + MASK;
    printf("After %lx \n", state->Regs[Rd]);
    if (!(BITsf & instruction)) {
      state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
    };
};

// Function that determines what type of wideMove operation is to be performed
static void wideMove(struct CompState* state, int32_t instruction) {
    int hw = (instruction >> 21) & 3;
    int opc = (instruction >> 29) & 3;
    int64_t imm16MASK = 65535; // 2 ^ 16 - 1
    int64_t imm16 = lsr_32(instruction, 5) & imm16MASK;
    int64_t Op = imm16 <<( hw * 16);
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
void determineTypeImmediate(struct CompState* state, int32_t instruction) {
    if (instruction & (BIT24)) {
        arithmeticImmediate(state, instruction);
    } else if (((instruction >> 23) & 5) == 5) {
        wideMove(state, instruction);
    };
};
