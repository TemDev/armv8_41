#include <stdio.h>
#include <stdbool.h>
#include "Definitions.h"

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


void andd(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->SP & Op;
        } else {
            result = state->Regs[Rn] & Op;
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                state->SP = result;
            } else {
                state->SP = result & BIT64MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT64MASK;
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->SP & BIT32MASK) & Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) & Op;
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                int res = result;
                state->SP = res;
            } else {
                state->SP = result & BIT32MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT32MASK;
        };
    };

    
};

// All the possible commands that might be asked to be performed
void bic(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->SP & (~Op);
        } else {
            result = state->Regs[Rn] & (~Op);
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                state->SP = result;
            } else {
                state->SP = result & BIT64MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT64MASK;
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->SP & BIT32MASK) & (~Op);
        } else {
            result = (state->Regs[Rn] & BIT32MASK) & (~Op);
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                int res = result;
                state->SP = res;
            } else {
                state->SP = result & BIT32MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT32MASK;
        };
    };
};

void orr(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->SP | Op;
        } else {
            result = state->Regs[Rn] | Op;
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                state->SP = result;
            } else {
                state->SP = result & BIT64MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT64MASK;
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->SP & BIT32MASK) | Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) | Op;
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                int res = result;
                state->SP = res;
            } else {
                state->SP = result & BIT32MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT32MASK;
        };
    };
};

void orn(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->SP | (~Op);
        } else {
            result = state->Regs[Rn] | (~Op);
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                state->SP = result;
            } else {
                state->SP = result & BIT64MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT64MASK;
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->SP & BIT32MASK) | (~Op);
        } else {
            result = (state->Regs[Rn] & BIT32MASK) | (~Op);
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                int res = result;
                state->SP = res;
            } else {
                state->SP = result & BIT32MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT32MASK;
        };
    };
};

void eor(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->SP ^ Op;
        } else {
            result = state->Regs[Rn] ^ Op;
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                state->SP = result;
            } else {
                state->SP = result & BIT64MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT64MASK;
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->SP & BIT32MASK) ^ Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) ^ Op;
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                int res = result;
                state->SP = res;
            } else {
                state->SP = result & BIT32MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT32MASK;
        };
    };
};

void eon(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->SP ^ (~Op);
        } else {
            result = state->Regs[Rn] ^ (~Op);
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                state->SP = result;
            } else {
                state->SP = result & BIT64MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT64MASK;
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->SP & BIT32MASK) ^ (~Op);
        } else {
            result = (state->Regs[Rn] & BIT32MASK) ^ (~Op);
        };
        
        if (Rd == BITrd) {
            if (result < 0) {
                int res = result;
                state->SP = res;
            } else {
                state->SP = result & BIT32MASK;
            };
        } else {
            state->Regs[Rd] = result & BIT32MASK;
        };
    };
};

void ands(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->ZR & Op;
        } else {
            result = state->Regs[Rn] & Op;
        };
        
        //Setting Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        if (result > 0) {
            state->PSTATE.C = result & BIT64 ? true : false;
        } else {
            state->PSTATE.C = false;
        };
        state->PSTATE.V = state->PSTATE.C;
        if (!(Rd == BITrd)) {
            if (result < 0) {
                state->Regs[Rd] = result;
            } else {
                state->Regs[Rd] = result & BIT64MASK;
            };
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->ZR & BIT32MASK) & Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) & Op;
        };
        
        //Setting Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        if (result > 0) {
            state->PSTATE.C = result & BIT32 ? true : false;
        } else {
            state->PSTATE.C = false;
        };
        state->PSTATE.V = state->PSTATE.C;
        
        if (!(Rd == BITrd)) {
            if (result < 0) {
                int res = result;
                state->Regs[Rd] = res;
            } else {
                state->Regs[Rd] = result & BIT32MASK;
            };
        };
    };


};

void bics(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == BITrd) {
            result = state->ZR & (~Op);
        } else {
            result = state->Regs[Rn] & (~Op);
        };
        
        //Setting Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        state->PSTATE.C = 0;
        state->PSTATE.V = 0;
        if (!(Rd == BITrd)) {
            if (result < 0) {
                state->Regs[Rd] = result;
            } else {
                state->Regs[Rd] = result & BIT64MASK;
            };
        };
        
    } else {
        long result;
        if (Rn == BITrd) {
            result = (state->ZR & BIT32MASK) & (~Op);
        } else {
            result = (state->Regs[Rn] & BIT32MASK) & (~Op);
        };
        
        //Setting Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        state->PSTATE.C = 0;
        state->PSTATE.V = 0;
        
        if (!(Rd == BITrd)) {
            if (result < 0) {
                int res = result;
                state->Regs[Rd] = res;
            } else {
                state->Regs[Rd] = result & BIT32MASK;
            };
        };
    };

};


// Multiply-Add: Rd := Ra + (Rn * Rm)
int madd(instruction) {
    char Ra = BITra & instruction;
    char Rn = BITrn & instruction;
    char Rm = BITrm & instruction;
    return Ra + (Rn * Rm);
}

// Multiply-Sub: Rd := Ra - (Rn * Rm)
int msub(instruction) {
    char Ra = BITra & instruction;
    char Rn = BITrn & instruction;
    char Rm = BITrm & instruction;
    return Ra - (Rn * Rm);
}

// Arithmetic-type commands - when the format is suitable to the one in specification and N = 0
void arithmetic(struct CompState* state, int instruction) {
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
void logical(struct CompState* state, int instruction) {
    char Opnew = ror_64 ((instruction & BITrm), (instruction & BIToperand));
    char Rn = (BITrn & instruction) >> 5;
    // Bit wise shift should be included and some things will be added / altered
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        bics(state, instruction, Rn, Opnew);
        break;
        case 2:
        eon(state, instruction, Rn, Opnew);
        break;
        case 1:
        orn(state, instruction, Rn, Opnew);
        break;
        default:
        bic(state, instruction, Rn, Opnew);
    }
};


void multiply(struct CompState* state, int instruction) {
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

void main() {
    
}