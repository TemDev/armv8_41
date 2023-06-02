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

static void add(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == REGISTER31) {
            result = state->SP + Op;
            state->SP = result;
        } else {
            result = state->Regs[Rn] + Op;
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
            state->SP = result;
            state->SP = state->SP & (BIT32 - 1);
        } else {
            if (state->Regs[Rd] & BIT31) {
                result = state->Regs[Rd] | BIT6432;
            } else {
                result = state->Regs[Rd] & (BIT32 - 1);
            };
            result += Op;
            state->Regs[Rd] = result;
            state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
        };
    };
};

static void adds(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == REGISTER31) {
            result = state->ZR + Op;
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
            result = state->Regs[Rn] + Op;
            state->PSTATE.V = (state->Regs[Rd] > 0 && Op > 0 && result < 0) || (state->Regs[Rd] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rd] < 0 && Op < 0) || (state->Regs[Rd] < 0 && Op > 0 && result >= 0) || (state->Regs[Rd] > 0 && Op < 0 && result >= 0);
            state->Regs[Rd] = result;
        };
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
        
    } else {
        int result;
        if (Rn == REGISTER31) {
	        if (state->ZR & BIT31) {
	            result = state->ZR | BIT6432;
	        } else {
                result = state->ZR & (BIT32 - 1);
	        };
	        result += Op;
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
	        if (state->Regs[Rd] & BIT31) {
	            result = state->Regs[Rd] | BIT6432;
	        } else {
	            result = state->Regs[Rd] & (BIT32 - 1);
	        };
	        result += Op;
            state->PSTATE.V = (state->Regs[Rd] > 0 && Op > 0 && result < 0) || (state->Regs[Rd] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rd] < 0 && Op < 0) || (state->Regs[Rd] < 0 && Op > 0 && result >= 0) || (state->Regs[Rd] > 0 && Op < 0 && result >= 0);
	        state->Regs[Rd] = result;
	        state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
        };
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
    };
};

static void arithmeticImmediate(struct CompState* state, int instruction) {
    const int sh = 4194304;
    const int imm12 = 4293280;
    const short rn = 992;
    int Op = ((imm12 & instruction) >> 10) << (12 * ((sh & instruction) >> 31));
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
        state->Regs[Rd] = ~Op;
    } else { // 32 bit mode.
        int op = Op;
        state->Regs[Rd] = ~op;
    };
};

static void movz(struct CompState* state, int instruction, long Op) {
    char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
        state->Regs[Rd] = Op;
    } else {
        int op = Op;
        state->Regs[Rd] = op;
    };
};

static void movk(struct CompState* state, int instruction, int imm16, char hw) {
    char Rd = BITrd & instruction;
    const long MASK = imm16 * pow(2, hw * 16);
    state->Regs[Rd] = state->Regs[Rd] ^ MASK;
};

static void wideMove(struct CompState* state, int instruction) {
    char hw = (instruction >> 21) & 3;
    char opc = (instruction >> 29) & 3;
    short imm16MASK = 65535; // 2 ^ 16 - 1
    int imm16 = (instruction >> 5) & imm16MASK;
    long Op = lsl_64(imm16, hw * 16);
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
    if (instruction & (BIT24)) {
        arithmeticImmediate(state, instruction);
    } else if (instruction & (BIT2523) == BIT2523) {
        wideMove(state, instruction);
    };
};

void main() {
    
};
