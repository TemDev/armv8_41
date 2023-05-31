#include <stdio.h>
#include <stdbool.h>
#include "bitwise.c"

#define BIT24 16777216 // == 2 ^ 24
#define BUTM 268435456 // == 2 ^ 28
#define BITshift 50331648 // 2 ^ 25 + 2 ^ 24
#define BITsf 2147483648 // == 2 ^ 31
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0
#define BITx 32768 // == 2 ^ 15
#define BITra 31744 // == 2 ^ 14 + 2 ^ 13 + 2 ^ 12 + 2 ^ 11 + 2 ^ 10
#define BITrn 992 // == 2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5
#define BITrm 2031616 // == 2 ^ 20 + 2 ^ 19 + 2 ^ 18 + 2 ^ 17 + 2 ^ 16
#define rd 4294967295 // 2 ^ 32 - 1
#define BIToperand 64512 // 2 ^ 15 + 2 ^ 14 + 2 ^ 13 + 2 ^ 12 + 2 ^ 11 + 2 ^ 10
#define NUM_GEN_REG 32

struct Flags {
  bool N, Z, C, V;
};

struct CompState {
  long ZR;
  long PC;
  struct Flags PSTATE;
  long SP;
  long Regs[NUM_GEN_REG];
};

void arithmeticc(struct CompState* state, int instruction) {
    const long fst = 166777216; // 2 ^ 24
    const long shift = 12582912; // 2 ^ 23 + 2 ^ 22
    const long lst = 2097152; // 2 ^ 21;
    long Op = ((imm12 & instruction) >> 10) << (12 * ((sh & instruction) >> 31));
    char Rn = (rn & instruction) >> 5;
    // Bit wise shift should be included and some things will be added / altered
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 7:
        bics(state, instruction, Rn, Op);
        break;
        case 6:
        ands(state, instruction, Rn, Op);
        break;
        case 5:
        eor(state, instruction, Rn, Op);
        break;
        case 4:
        eon(state, instruction, Rn, Op);
        break;
        case 3:
        orn(state, instruction, Rn, Op);
        break;
        case 2:
        orr(state, instruction, Rn, Op);
        break;
        case 1:
        bic(state, instruction, Rn, Op);
        break;
        case 0:
        andd(state, instruction, Rn, Op);
        break;
        default:
        add(state, instruction, Rn, Op);
    }
};


void andd(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] & Op;
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
    
};

void bic(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] & (~Op);
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
};

void orr(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] | Op;
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
};

void orn(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] | (~Op);
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
};

void eon(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] ^ (~Op);
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
};

void eor(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] ^ Op;
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
};

void ands(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        if (Rn == 31) {
            state->Regs[Rd] = state->SP & Op;
        } else {
            state->Regs[Rd] = state->Regs[Rn] + Op;
        };
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    } else {
        state->SP = state->Re
    };

};

void bics(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        if (Rn == 31) {
            state->Regs[Rd] = state->SP & (!Op);
        } else {
            state->Regs[Rd] = state->Regs[Rn] + Op2;
        };
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    } else {
        state->SP = state->Re
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

void logic(struct CompState* state, int instruction) {
    if ((instruction >> 22) & 1) {
        lsr ((instruction & BIToperand) 1) // assumed that all the shifts happen by 1 bit
    } else if ((instruction >> 22) & 2) {
        asr ((instruction & BIToperand) 1)
    } else
        lsl ((instruction & BIToperand) 1)
}

void bitLogic(struct CompState* state, int instruction) {
    ror ((instruction & BIToperand) 1)
};

void multiply(struct CompState* state, int instruction) {
  if (instruction & BITx) {
      state->Regs[Rd] = msub(instruction);
  } else {
      state->Regs[Rd] = madd(instruction);
  }
};

// Determines type of immediate instruction, arithmetic or wideMove.
void determineType(struct CompState* state, int instruction) {
    if (instruction & !(BITM) & (BIT24)) {
        arithmetic(state, instruction);
    } else if (instruction & !(BITM) & !(BIT24) {
        logic(state, instruction);
        else if (instruction & (BITM) & (BIT24)) {
            multiply(state, instruction);
        }
    };
};

void main() {
    
}


