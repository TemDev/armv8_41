// Handles register instruction by determining their type and updating appropriate registers and flags

#include <stdio.h>
#include <stdbool.h>
#include "RegisterInstructionProcessing.h"
#include "bitwise.h"
#include "immediateInstructionProcessing.h"

#define BITsf 2147483648 // == 2 ^ 31, MASK
#define BITrd 31         // == 0x1F, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK

//Function pointer for all types of shifts performed when dealing with arithmetic and logical instructions
typedef int64_t (*bitwise_pointer)(int64_t, int);
typedef int32_t (*bitwise_pointer2)(int64_t, int);

/* All logical functions are similar in terms of code needed to imnplement them.
 * in order to avoid repetition, the functions below */


// Bitwise AND operation
static int64_t LOCAL_AND(int64_t x, int64_t y) {
    return x & y;
}

// Bitwise inclusive OR
static int64_t LOCAL_IOR(int64_t x, int64_t y) {
    return x | y;
}

//Bitwise exlusive OR 
static int64_t LOCAL_EOR(int64_t x, int64_t y) {
    return x ^ y;
}

/* Standard look of the function for the logical functions. Correct operators will the applied using the function pointers above. 
 * Op will be negated in required cases to perform the operations using negated operand correctly */

static int64_t bin_op(CompState* state, int32_t instruction, unsigned char Rn, int64_t Op, int64_t (*fn)(int64_t, int64_t)) {
    unsigned char Rd = BITrd & instruction;
    int64_t result;
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
    return result;
}

//ands function which uses the and function inside it and then sets the flags appropriatelly 
static void ands(CompState* state, int32_t instruction, unsigned char Rn, int64_t Op) {
    int64_t resultMain = bin_op(state, instruction, Rn, Op, LOCAL_AND);
    if ((instruction & BITsf) == 0)  {
         int32_t result = (int32_t) (resultMain);
         state->PSTATE.N = result < 0;       
    } else {
         state->PSTATE.N = resultMain < 0;
    }
    state->PSTATE.Z = resultMain == 0;
    state->PSTATE.C = 0;
    state->PSTATE.V = 0;
}


// Logical instructions processing 
static void logical(CompState* state, int32_t instruction, unsigned char Rm, unsigned char Rn, unsigned char shift, unsigned char shiftBy) {
    int64_t Opnew;
    bitwise_pointer bitwise_func;
    bitwise_pointer2 bitwise_func2;

    //Using a function pointer defined at the top of the file to choose which shift to perform
    if (!(instruction & BITsf)) {
        Opnew = (int32_t) state->Regs[Rm];
        if (shift == 0) {
             bitwise_func2 = (&lsl_32);
        } else if (shift == 1) {
	     bitwise_func2 = (&lsr_32);
        } else if (shift == 2) {
             bitwise_func2 = (&asr_32);
        } else {
             bitwise_func2 = (&ror_32);
        }
	Opnew = bitwise_func2(Opnew, shiftBy);
    } else {
        Opnew = state->Regs[Rm];
        if (shift == 0) {
             bitwise_func = (&lsl_64);
        } else if (shift == 1) {
             bitwise_func = (&lsr_64);
        } else if (shift == 2) {
             bitwise_func = (&asr_64);
        } else {
             bitwise_func = (&ror_64);
        }
	Opnew = bitwise_func(Opnew, shiftBy);
    }
    //Determine which logical operation to perform 
    /* To determine the type of logical instruction to perform we need opc and N which are in different places in the instruction
     * To find opc shift the instruction by 28 and bitwise and with 6 to free up the last bit. 
     * To find the N bit we shift the instruction by 21 bit to the right.
     * Using bitwise or operation put them together  */
    unsigned char opcN = ((instruction >> 28) & 6) | ((instruction >> 21) & 1);
    switch (opcN) {
        case 7:
            ands(state, instruction, Rn, (~Opnew));
            break;
        case 6:
            ands(state, instruction, Rn, (Opnew));
            break;
        case 5:
	    bin_op(state, instruction, Rn, (~Opnew), LOCAL_EOR);
            break;
        case 4:
            bin_op(state, instruction, Rn, (Opnew), LOCAL_EOR);
            break;
        case 3:
            bin_op(state, instruction, Rn, (~Opnew), LOCAL_IOR);
            break;
        case 2:
            bin_op(state, instruction, Rn, (Opnew), LOCAL_IOR);
            break;
        case 1:
            bin_op(state, instruction, Rn, (~Opnew), LOCAL_AND);
            break;
        default:
            bin_op(state, instruction, Rn, (Opnew), LOCAL_AND);
    }
}

// Arithmatic instructions processing
static void arithmetic(CompState* state, int32_t instruction, unsigned char Rm, unsigned char Rn, unsigned char shift, unsigned char shiftBy) {
    int64_t Opnew;

    //Using a function pointer defined at the top of the file to perform the required shift operation
    bitwise_pointer bitwise_func;
    bitwise_pointer2 bitwise_func2;
    
    if (!(instruction & BITsf)) {
        if (shift == 0) {
             bitwise_func2 = (&lsl_32);
        } else if (shift == 1) {
             bitwise_func2 = (&lsr_32);
        } else if (shift == 2) {
             bitwise_func2 = (&asr_32);
        }
    } else {
        if (shift == 0) {
             bitwise_func = (&lsl_64);
        } else if (shift == 1) {
             bitwise_func = (&lsr_64);
        } else if (shift == 2) {
             bitwise_func = (&asr_64);
        }
    };
    if (shift == 3) {
        Opnew = state->Regs[Rm];
    } else {
        if (!(instruction & BITsf)) {
	    Opnew = (int64_t) bitwise_func2((state->Regs[Rm]), shiftBy);
        } else {
            Opnew = (int64_t) bitwise_func((state->Regs[Rm]), shiftBy);
        }
    };

    // Choice for the arithmetic operation to perform
    unsigned char opc = (instruction >> 29) & 3; // Opc if found by shifting the instruction by 29 bits and then biwise and it with 3 to see what instructon to perform
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

//Multiply instructions processing
static void multiply(CompState* state, int32_t instruction, int32_t Rd, int32_t Ra, int64_t Rrm, int64_t Rrn, int64_t Rra, int64_t mask) {
  if (!(instruction & BITsf)) {
    mask = BIT32 - 1;
    Rrm = Rrm & (BIT32 - 1);
    Rrn = Rrn & (BIT32 - 1);
    Rra = Rra & (BIT32 - 1);
  }
  if (1 & (instruction>>15)) { // shifting the instruction by 15 to get the first bit pf the operand and bitwise and it with 1
    if (Ra == 31) {
      state->Regs[Rd] = state->ZR - (mask & ( Rrn * Rrm));
      } else {
	state->Regs[Rd] = Rra -(mask & (Rrn * Rrm));
      };
  } else {
      state->Regs[Rd] = Rra + (mask & (Rrn * Rrm));
  };
  if (!(instruction & BITsf)) {
    state->Regs[Rd] = (state -> Regs[Rd]) & (BIT32 - 1); 
  }
};

// Determines type of Register instruction - arithmetic, logical or multiplication.
void determineTypeRegister(CompState* state, int32_t instruction) {

    unsigned char shift = (instruction >> 22) & 3; // Shifting the instruction by 22 to get the "shift" and determine which value to shift
    unsigned char shiftBy = ((63) & (instruction >> 10)); // shiftBy is formed making a 64 (0-63)bit mask from the instruction shifted by 10
    unsigned char m = 1 & (instruction >> 28); // Shifting the instruction by 28 to get the M bit
    unsigned char r24 = 1 & (instruction >> 24); // Shifting the instruction by 24 to get the first bit of the opr value
    int32_t Rd = (int32_t)((31) & instruction); // Rd is formed making a 32 (0-31)bit 
    int32_t Ra = (int32_t)((31) & (instruction>>10)); // Ra is formed making a 32 (0-31) bit mask from the instruction shifted by 10
    int32_t Rn = (int32_t)((31) & (instruction>>5)); // Rn is formed making a 32 (0-31) bit mask rom the instruction shifted by 5
    int32_t Rm = (int32_t)((31) & (instruction>>16)); // Rm is formed making a 32 (0-31)bit mask from the instruction shifted by 16
    int64_t Rrm  = state-> Regs[Rm];
    int64_t Rrn  = state-> Regs[Rn];
    int64_t Rra  = state-> Regs[Ra];
    int64_t mask = -1; // creating a mask of -1 for further use
    if ((m == 0) && (r24 == 1)) {
        arithmetic(state, instruction, Rm, Rn, shift, shiftBy);
    } else if ((m == 0) && (r24 == 0)) {
        logical(state, instruction, Rm, Rn, shift, shiftBy);
    } else if ((m == 1) && (r24 == 1)) {
	multiply(state, instruction, Rd, Ra, Rrm, Rrn, Rra, mask);
    }
};

