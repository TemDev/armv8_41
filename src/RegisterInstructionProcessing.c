#include <stdio.h>
#include <stdbool.h>
#define BIT24 16777216 // == 2 ^ 24
#define BUTM 268435456 // == 2 ^ 28
#define BIT2523 41943040 // == 2 ^ 25 + 2 ^ 23
#define BITsf 2147483648 // == 2 ^ 31
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0
#define rd 4294967295; // 2 ^ 32 - 1
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

void arithmetic(struct CompState* state, int instruction) {
    const int fst = 166777216; // 2 ^ 24
    const int shift = 12582912; // 2 ^ 23 + 2 ^ 22
    const short lst = 2097152; // 2 ^ 21;
    int Op = ((imm12 & instruction) >> 10) << (12 * ((sh & instruction) >> 31));
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

void arithmetic(struct CompState* state, int instruction) {
    const int sh = 4194304;
    const int imm12 = 4293280;
    const short rn = 992;
    int Op = ((imm12 & instruction) >> 10) << (12 * ((sh & instruction) >> 31));
    char Rn = (rn & instruction) >> 5;
    char opc = (instruction >> 29) & 3;
    switch (opc) {
        case 3:
        subs(state, instruction, Rn, Op);
        break;
        case 2:
        sub(state, instruction, Rn, Op);
        break;
        case 1:
        adds(state, instruction, Rn, Op);
        break;
        default:
        add(state, instruction, Rn, Op);
    }
};

// Multiply-Add: Rd := Ra + (Rn * Rm)
int madd(int Ra, int Rn, int Rm) {
    return Ra + (Rn * Rm);
}

// Multiply-Sub: Rd := Ra - (Rn * Rm)
int msub(int Ra, int Rn, int Rm) {
    return Ra - (Rn * Rm);
}


void bit-logic(struct CompState* state, int instruction) {
    
};

void multiply(struct CompState* state, int instruction) {
    
};

// Determines type of immediate instruction, arithmetic or wideMove.
void determineType(struct CompState* state, int instruction) {
    if (instruction & !(BITM) & (BIT24)) {
        arithmetic(state, instruction);
    } else if (instruction & !(BITM) & !(BIT24) {
        bit-logic(state, instruction);
        else if (instruction & (BITM) & (BIT24)) {
            multiply(state, instruction);
        }
    };
};

void main() {
    
}


