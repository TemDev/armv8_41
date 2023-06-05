#include <stdio.h>
#include <stdbool.h>
#include "RegisterInstructionProcessing.h"
#include "bitwise.h"

#define BIT24 16777216 // == 2 ^ 24, MASK
#define BITm 268435456 // == 2 ^ 28, MASK
#define BITsf 2147483648 // == 2 ^ 31, MASK
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0, MASK
#define BITx 32768 // == 2 ^ 15, MASK
#define BITra 31744 // == 2 ^ 14 + 2 ^ 13 + 2 ^ 12 + 2 ^ 11 + 2 ^ 10, MASK
#define BITrn 992 // == 2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5, MASK
#define BITrm 2031616 // == 2 ^ 20 + 2 ^ 19 + 2 ^ 18 + 2 ^ 17 + 2 ^ 16, MASK
#define BIToperand 64512 // 2 ^ 15 + 2 ^ 14 + 2 ^ 13 + 2 ^ 12 + 2 ^ 11 + 2 ^ 10, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK
#define BIT6432 0b1111111111111111111111111111111100000000000000000000000000000000
#define BIT31 2147483648 // == 2 ^ 31, MASK



// All the possible commands that might be asked to be performed

//Only functions and orr eor and ands are written below because the other function
//are achieved by passing a negated Op parameter to these functions. This significantly simplifies the code

//Bitwise AND

static int LOCAL_ADD(long x, long y) {
    return x + y;
}

static int LOCAL_AND(long x, long y) {
    return x & y;
}

// Bitwise inclusive OR
static int LOCAL_IOR(long x, long y) {
    return x | y;
}

//Bitwise eclusive OR function 
static int LOCAL_EOR(long x, long y) {
    return x ^ y;
}

// Standard look of the finction and, orr, eon. Correct operators will the applied using the function pointers above and Op will be negated in required cases to chieve bic, orn and eor
static void bin_op1(struct CompState* state, int instruction, char Rn, int Op, int (*fn)(long, long)) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == BITrd) {
            result = (*fn)(state->SP, Op);
            state->SP = result;
        } else {
            result = (*fn)(state->SP, Op);
            state->Regs[Rd] = result;
        };
        
    } else {
        int result;
        if (Rn == BITrd) {
	        if (state->SP & BIT31) {
                result = state->SP | BIT6432;
	        } else {
                result = state->SP & (BIT32 - 1);
	        };
            result = (*fn)(result, Op);
              state->SP = result;
            state->SP = state->SP & (BIT32 - 1);
        } else {
            if (state->Regs[Rd] & BIT31) {
                result = state->Regs[Rd] | BIT6432;
            } else {
                result = state->Regs[Rd] & (BIT32 - 1);
            };
            result = (*fn)(result, Op);
            state->Regs[Rd] = result;
            state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
        };
    }
}

static void bin_op2(struct CompState* state, int instruction, char Rn, int Op, int (*fn)(long, long)) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long result;
        if (Rn == BITrd) {
            result = (*fn)(state->ZR, Op);
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
            result = (*fn)(state->Regs[Rn], Op);
            state->PSTATE.V = (state->Regs[Rd] > 0 && Op > 0 && result < 0) || (state->Regs[Rd] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rd] < 0 && Op < 0) || (state->Regs[Rd] < 0 && Op > 0 && result >= 0) || (state->Regs[Rd] > 0 && Op < 0 && result >= 0);
            state->Regs[Rd] = result;
        };
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
        
    } else {
        int result;
        if (Rn == BITrd) {
	        if (state->ZR & BIT31) {
	            result = state->ZR | BIT6432;
	        } else {
                result = state->ZR & (BIT32 - 1);
	        };
	        result = (*fn)(result, Op);
            state->PSTATE.V = (state->ZR > 0 && Op > 0 && result < 0) || (state->ZR < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->ZR < 0 && Op < 0) || (state->ZR < 0 && Op > 0 && result >= 0) || (state->ZR > 0 && Op < 0 && result >= 0);
        } else {
	        if (state->Regs[Rd] & BIT31) {
	            result = state->Regs[Rd] | BIT6432;
	        } else {
	            result = state->Regs[Rd] & (BIT32 - 1);
	        };
	        result = (*fn)(result, Op);
            state->PSTATE.V = (state->Regs[Rd] > 0 && Op > 0 && result < 0) || (state->Regs[Rd] < 0 && Op < 0 && result > 0);
            state->PSTATE.C = (state->Regs[Rd] < 0 && Op < 0) || (state->Regs[Rd] < 0 && Op > 0 && result >= 0) || (state->Regs[Rd] > 0 && Op < 0 && result >= 0);
	        state->Regs[Rd] = result;
	        state->Regs[Rd] = state->Regs[Rd] & (BIT32 - 1);
        };
        state->PSTATE.N = result < 0;
        state->PSTATE.Z = result == 0;
    };
};

static void andd(struct CompState* state, int instruction, char Rn, int Op) {
	            return bin_op1(state, instruction, Rn, Op, LOCAL_AND);
}

static void orr(struct CompState* state, int instruction, char Rn, int Op) {
	            return bin_op1(state, instruction, Rn, Op, LOCAL_IOR);
}

static void eor(struct CompState* state, int instruction, char Rn, int Op) {
	            return bin_op1(state, instruction, Rn, Op, LOCAL_EOR);
}

static void add(struct CompState* state, int instruction, char Rn, int Op) {
	            return bin_op1(state, instruction, Rn, Op, LOCAL_ADD);
}

static void adds(struct CompState* state, int instruction, char Rn, int Op) {
		            return bin_op2(state, instruction, Rn, Op, LOCAL_ADD);
}

static void ands(struct CompState* state, int instruction, char Rn, int Op) {
    andd(state, instruction, Rn, Op);
    char Rd = BITrd & instruction;
    int result = state -> Regs[Rd]; 
    state->PSTATE.N = result < 0;
    state->PSTATE.Z = result == 0;
    state->PSTATE.C = 0;
    state->PSTATE.V = 0;
}

// Arithmetic-type commands - when the format is suitable to the one in specification and N = 0
static void logical(struct CompState* state, int instruction) {
    long Opnew;
    char Rm = ((31) & (instruction>>16));
    if ((instruction >> 22) & 3 == 0) {
        Opnew = lsl_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else if ((instruction >> 22) & 3 == 1) {
        Opnew = lsr_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else if ((instruction >> 22) & 3 == 2) {
        Opnew = asr_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else {
	Opnew = ror_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    }
    char Rn = ((31) & (instruction>>5));
    char opcN = ((instruction >> 28) & 6) | ((instruction >> 24) & 1);
    printf("Rn:%d opc:%d\n", Rn, opcN);
    switch (opcN) {
        case 7:
        ands(state, instruction, Rn, (~Opnew));
        break;
        case 6:
        ands(state, instruction, Rn, Opnew);
        break;
        case 5:
        eor(state, instruction, Rn, (~Opnew));
        break;
        case 4:
        eor(state, instruction, Rn, Opnew);
        break;
        case 3:
        orr(state, instruction, Rn, (~Opnew));
        break;
        case 2:
        orr(state, instruction, Rn, Opnew);
        break;
        case 1:
        andd(state, instruction, Rn, (~Opnew));
        break;
        default:
        andd(state, instruction, Rn, Opnew);
    }
}

//Logical commands (Bit-logic commands) - when the format is as required in specification and N = 1
static void arithmetic(struct CompState* state, int instruction) {
    long Opnew;
    char Rm = ((31) & (instruction>>16));
    if ((instruction >> 22) & 3 == 0) {
	Opnew = lsl_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else if ((instruction >> 22) & 3 == 1) {
	Opnew = lsr_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else if ((instruction >> 22) & 3 == 2) {
	Opnew = asr_64 ((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else {
	Opnew = state->Regs[Rm];
    }

    char Rn = ((31) & (instruction>>5));
    // Bit wise shift should be included and some things will be added / altered
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        adds(state, instruction, Rn, (-Opnew));
        break;
        case 2:
        add(state, instruction, Rn, (-Opnew));
        break;
        case 1:
        adds(state, instruction, Rn, Opnew);
        break;
        default:
        add(state, instruction, Rn, Opnew);
    }
};


static void multiply(struct CompState* state, int instruction) {
  int Rd = (int)((31) & instruction);
  int Ra = (int)((31) & (instruction>>10));
  int Rn = (int)((31) & (instruction>>5));
  int Rm = (int)((31) & (instruction>>16));
  printf("Rd:%d Ra:%d Rn:%n Rm:%d\n", Rd, Ra, Rn, Rm);
/* if (1 & (instruction>>15)) {
      state->Regs[Rd] = state->Regs[Ra] - (state->Regs[Rn] * state->Regs[Rm]);
  } else ;{
      state->Regs[Rd] = state->Regs[Ra] + (state->Regs[Rn] * state->Regs[Rm]);
  }*/
};

// Determines type of Register instruction - arithmetic, logical or multiplication.
void determineTypeRegister(struct CompState* state, int instruction) {
    char m = 1 & (instruction>>28);
    char r24 = 1 & (instruction>>24);
    printf("m:%d R24:%d\n", m, r24);
    printf("Printing\n");
    if ((m == 0) && (r24 == 1)) {
	    printf("Arithmetic\n");
        arithmetic(state, instruction);
	printf("Arithmetic\n");
    } else if ((m == 0) && (r24 == 0)) {
        logical(state, instruction);
	printf("Logical\n");
    }
        else if ((m == 1) && (r24 == 1)) {
            printf("Multiply\n");
	    multiply(state, instruction);
	    printf("Multiply\n");
        }
    else {
	    printf("Nothing\n");
    }
};

static void main() {
    
}




