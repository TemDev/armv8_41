#include <stdio.h>
#include <stdbool.h>
#include "RegisterInstructionProcessing.h"
#include "bitwise.h"

#define BIT24 16777216 // == 2 ^ 24
#define BITm 268435456 // == 2 ^ 28
#define BITshift 50331648 // 2 ^ 25 + 2 ^ 24
#define BITsf 2147483648 // == 2 ^ 31
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0
#define BITx 32768 // == 2 ^ 15
#define BITra 31744 // == 2 ^ 14 + 2 ^ 13 + 2 ^ 12 + 2 ^ 11 + 2 ^ 10
#define BITrn 992 // == 2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5
#define BITrm 2031616 // == 2 ^ 20 + 2 ^ 19 + 2 ^ 18 + 2 ^ 17 + 2 ^ 16
#define rd 4294967295 // 2 ^ 32 - 1
#define BIToperand 64512 // 2 ^ 15 + 2 ^ 14 + 2 ^ 13 + 2 ^ 12 + 2 ^ 11 + 2 ^ 10
#define BIT32MASK 4294967295 // == 2 ^ 32 - 1, MASK
#define BIT64MASK 18446744073709551615 // == 2 ^ 64 - 1, MASK
#define BIT64 18446744073709551616 // == 2 ^ 64, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK
#define REGISTER31 31
#define BIT6432 0b1111111111111111111111111111111100000000000000000000000000000000
#define BIT31 2147483648 // == 2 ^ 31, MASK



// All the possible commands that might be asked to be performed

//Only functions and orr eor and ands are written below because the other function
//are achieved by passing a negated Op parameter to these functions. This significantly simplifies the code

static int LOCAL_AND(int x, int y) {
    return x & y;
}

static int LOCAL_OR(int x, int y) {
    return x | y;
}

static int LOCAL_BAND(int x, int y) {
    return x ^ y;
}

static void andd(struct CompState* state, int instruction, char Rn, int Op) {
    return bin_op1(state, instruction, Rn, Op, LOCAL_AND);
}

static void orr(struct CompState* state, int instruction, char Rn, int Op) {
    return bin_op1(state, instruction, Rn, Op, LOCAL_OR);
}

static void eor(struct CompState* state, int instruction, char Rn, int Op) {
    return bin_op1(state, instruction, Rn, Op, LOCAL_BAND);
}

static void bin_op1(struct CompState* state, int instruction, char Rn, int Op, int *fn(int, int)) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == REGISTER31) {
            result = state->ZR & Op;
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
            result = state->Regs[Rn] & Op;
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



static void ands(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == REGISTER31) {
            result = fn(state->ZR, Op);
            if ()
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
            result = fn(state->Regs[Rn], Op);
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
	        result = fn(result, Op);
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
	        if (state->Regs[Rd] & BIT31) {
	            result = state->Regs[Rd] | BIT6432;
	        } else {
	            result = state->Regs[Rd] & (BIT32 - 1);
	        };
	        result = fn(result, Op);
            state->PSTATE.V = (state->Regs[Rd] > 0 && Op > 0 && result < 0) || (state->Regs[Rd] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rd] < 0 && Op < 0) || (state->Regs[Rd] < 0 && Op > 0 && result >= 0) || (state->Regs[Rd] > 0 && Op < 0 && result >= 0);
	        state->Regs[Rd] = result;
	        state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
        };
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
    };
};

static void and_flag(struct CompState* state, int instruction, char Rn, int Op) {
    result = andd(state, instruction, Rn, (~Op));
    state->PSTATE.N = result < 0;
    state->PSTATE.Z = result == 0;
    state->PSTATE.C = 0;
    state->PSTATE.V = 0;
}


// Multiply-Add: Rd := Ra + (Rn * Rm)
static int madd(instruction) {
    char Ra = BITra & instruction;
    char Rn = BITrn & instruction;
    char Rm = BITrm & instruction;
    return Ra + (Rn * Rm);
}

// Multiply-Sub: Rd := Ra - (Rn * Rm)
static int msub(instruction) {
    char Ra = BITra & instruction;
    char Rn = BITrn & instruction;
    char Rm = BITrm & instruction;
    return Ra - (Rn * Rm);
}

// Arithmetic-type commands - when the format is suitable to the one in specification and N = 0
static void arithmetic(struct CompState* state, int instruction) {
    char Opnew = BITrm;
    if ((instruction >> 22) & 1) {
        Opnew = lsl_64 ((instruction & BITrm), (instruction & BIToperand));
    } else if ((instruction >> 22) & 2) {
        Opnew = lsr_64 ((instruction & BITrm), (instruction & BIToperand));
    } else {
        Opnew = asr_64 ((instruction & BITrm), (instruction & BIToperand));
    }
    char Rn = (BITrn & instruction) >> 5;
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        ands(state, instruction, Rn, Opnew);
        break;
        case 2:
        eor(state, instruction, Rn, Opnew);
        break;
        case 1:
        orr(state, instruction, Rn, Opnew);
        break;
        default:
        andd(state, instruction, Rn, Opnew);
    }
}

//Logical commands (Bit-logic commands) - when the format is as required in specification and N = 1
static void logical(struct CompState* state, int instruction) {
    char Opnew = ~(ror_64 ((instruction & BITrm), (instruction & BIToperand)));
    char Rn = (BITrn & instruction) >> 5;
    // Bit wise shift should be included and some things will be added / altered
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        ands(state, instruction, Rn, Opnew);
        break;
        case 2:
        eor(state, instruction, Rn, Opnew);
        break;
        case 1:
        orr(state, instruction, Rn, Opnew);
        break;
        default:
        andd(state, instruction, Rn, Opnew);
    }
};


static void multiply(struct CompState* state, int instruction) {
  char Rd = instruction & BITrd;
  if (instruction & BITx) {
      state->Regs[Rd] = msub(instruction);
  } else {
      state->Regs[Rd] = madd(instruction);
  }
};

// Determines type of immediate instruction, arithmetic or wideMove.
void determineTypeRegister(struct CompState* state, int instruction) {
    char m = instruction & BITm;
    char r24 = instruction & BIT24;
    if (instruction && (~m) && (~r24)) {
        arithmetic(state, instruction);
    } else if (instruction && (~m) && (~r24)) {
        logical(state, instruction);
    }
        else if (instruction && (m) && (r24)) {
            multiply(state, instruction);
        }
};

static void main() {
    
}




