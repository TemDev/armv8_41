#include <stdio.h>
#include <stdbool.h>
#include "RegisterInstructionProcessing.h"
#include "bitwise.h"
#include "immediateInstructionProcessing.h"

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

static long LOCAL_AND(long x, long y) {
    return x & y;
}

// Bitwise inclusive OR
static long LOCAL_IOR(long x, long y) {
    return x | y;
}

//Bitwise eclusive OR function 
static long LOCAL_EOR(long x, long y) {
    return x ^ y;
}

// Standard look of the finction and, orr, eon. Correct operators will the applied using the function pointers above and Op will be negated in required cases to chieve bic, orn and eor
static long bin_op1(struct CompState* state, int instruction, char Rn, long Op, long (*fn)(long, long)) {
    printf("bin_op Op: %ld\n", Op);
    char Rd = BITrd & instruction;
    long result;
    if (Rn == BITrd) {
        result = (*fn)(state->ZR, Op);
    } else {
        result = (*fn)(state->Regs[Rn], Op);
    }

    if (!(Rd == BITrd)) {
	if (instruction & BITsf) {
            state->Regs[Rd] = result;
	} else {
    	    state->Regs[Rd] = result & (BIT32 - 1);
	}
    }
    printf("result: %ld\n", result);
    return result;
}

static void ands(struct CompState* state, int instruction, char Rn, long Op) {
    long resultMain = bin_op1(state, instruction, Rn, Op, LOCAL_AND);
    char Rd = BITrd & instruction;
    if ((instruction & BITsf) == 0)  {
       int result = (int) (resultMain);
       printf("Register: %x\n", state->Regs[Rd]);
       printf("Result: %d \n", result);
       state->PSTATE.N = result < 0;       
    } else {
       state->PSTATE.N = resultMain < 0;
    }
    state->PSTATE.Z = resultMain == 0;
    state->PSTATE.C = 0;
    state->PSTATE.V = 0;
}

// Arithmetic-type commands - when the format is suitable to the one in specification and N = 0
static void logical(struct CompState* state, int instruction) {
    long Opnew;
    char Rm = ((31) & (instruction>>16));
    char shift = (instruction >> 22) & 3;
    printf("REGisTer: %ldx ", state->Regs[Rm]);
    if (instruction & BITsf == 0) {
      Opnew = (int) state->Regs[Rm];
    } else {
      Opnew = state->Regs[Rm];
    };
    if (shift == 0) {
        if (!(instruction & BITsf)) {
           Opnew = lsl_32 ((Opnew), ((63) & (instruction >> 10)));
        } else {
           Opnew = lsl_64 ((Opnew), ((63) & (instruction >> 10)));
        };
	printf("instruction >> 10 %ld\n", ((63) & (instruction >> 10)));
	printf("lsl Opnew: %ld\n", Opnew);
    } else if (shift  == 1) {
      printf("OPNeW: %ld\n", Opnew);
        if (!(instruction & BITsf)) {
           Opnew = lsr_32 ((Opnew), ((63) & (instruction >> 10)));
        } else {
           Opnew = lsr_64 ((Opnew), ((63) & (instruction >> 10)));
        };
        printf("instruction >> 10 %ld\n", ((63) & (instruction >> 10)));
	printf("lsr Opnew: %ld\n", Opnew);
    } else if (shift == 2) {
        printf("OPNEW: %ld\n", Opnew);
	if (!(instruction & BITsf)) {
           Opnew = asr_32 ((Opnew), ((63) & (instruction >> 10)));
        } else {
           Opnew = asr_64 ((Opnew), ((63) & (instruction >> 10)));
        };
	printf("instruction >> 10 %ld\n", ((63) & (instruction >> 10)));
	printf("asr Opnew: %ld\n", Opnew);
    } else {
        if (!(instruction & BITsf)) {
           Opnew = ror_32 ((Opnew), ((63) & (instruction >> 10)));
        } else {
           Opnew = ror_64 ((Opnew), ((63) & (instruction >> 10)));
        };
	printf("instruction >> 10 %ld\n", ((63) & (instruction >> 10)));
	printf("ror Opnew: %ld\n", Opnew);
    }
    char Rn = ((31) & (instruction>>5));
    char opcN = ((instruction >> 28) & 6) | ((instruction >> 21) & 1);
    printf("Rn:%d opc:%d\n", Rn, opcN);
    switch (opcN) {
        case 7:
        ands(state, instruction, Rn, (~Opnew));
        break;
        case 6:
        ands(state, instruction, Rn, (Opnew));
        break;
        case 5:
	bin_op1(state, instruction, Rn, (~Opnew), LOCAL_EOR);
        break;
        case 4:
        bin_op1(state, instruction, Rn, (Opnew), LOCAL_EOR);
        break;
        case 3:
	  printf("Vanya %ld\n", (~Opnew));
        bin_op1(state, instruction, Rn, (~Opnew), LOCAL_IOR);
        break;
        case 2:
        bin_op1(state, instruction, Rn, (Opnew), LOCAL_IOR);
        break;
        case 1:
        bin_op1(state, instruction, Rn, (~Opnew), LOCAL_AND);
        break;
        default:
	printf("RN: %d\n", Rn);
	printf("OPNEW: %ld \n", Opnew);
        bin_op1(state, instruction, Rn, (Opnew), LOCAL_AND);
    }
}

//Logical commands (Bit-logic commands) - when the format is as required in specification and N = 1
static void arithmetic(struct CompState* state, int instruction) {
    long Opnew;
    char Rm = ((31) & (instruction>>16));
    char shift = (instruction >> 22) & 3;
    if (shift == 0) {
      Opnew = (long) lsl_64((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else if (shift == 1) {
      Opnew = (long) lsr_64((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else if (shift  == 2) {
      Opnew = (long) asr_64((state->Regs[Rm]), ((63) & (instruction >> 10)));
    } else {
	Opnew = state->Regs[Rm];
    }

    char Rn = ((31) & (instruction>>5));
    // Bit wise shift should be included and some things will be added / altered
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        adds(state, instruction, Rn, (-Opnew), 1);
        break;
        case 2:
        add(state, instruction, Rn, (-Opnew));
        break;
        case 1:
        adds(state, instruction, Rn, Opnew, 0);
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
  printf("Rd:%d Ra:%d Rn:%d Rm:%d", Rd, Ra, Rn, Rm);
  long rrn, rrm, rra, mask;
  rrm  = state-> Regs[Rm];
  rrn  = state-> Regs[Rn];
  rra  = state-> Regs[Ra];
  mask = -1;
  if (!(instruction & BITsf)) {
    mask = BIT32 - 1;
    rrm = rrm & (BIT32 -1);
    rrn = rrn & (BIT32 -1);
    rra = rra & (BIT32 -1);
    
  }
  if (1 & (instruction>>15)) {
    if (Ra == 31) {
      state->Regs[Rd] = state->ZR - (mask & ( rrn * rrm));
      } else {
	state->Regs[Rd] = rra -(mask & (rrn * rrm));
      };
  } else {
      state->Regs[Rd] = rra + (mask & (rrn * rrm));
  };
  if (!(instruction & BITsf)) {
    state->Regs[Rd] = (state -> Regs[Rd]) & (BIT32 -1); 
  }
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
        printf("Logical\n");
        logical(state, instruction);
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



