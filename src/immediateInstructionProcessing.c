#include <stdio.h>
#include <stdbool.h>
#define BIT24 16777216 // == 2 ^ 24
#define BIT2523 41943040 // == 2 ^ 25 + 2 ^ 23
#define BITsf 2147483648 // == 2 ^ 31
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0
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


void add(struct CompState* state, int instruction, char Rn, int Op) {
    const int rd = 4294967295; // 2 ^ 32 - 1
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        state->Regs[Rd] = state->Regs[Rn] + Op;
        if (BITsf & instruction) {
            
        } else {
            state->Regs[Rd] = state->Regs[Rd] & rd;
        };
    }
    
};

void adds(struct CompState* state, int instruction, char Rn, int Op) {
    const int rd = 4294967295; // 2 ^ 32 - 1
    char Rd = BITrd & instruction;
    if (Rd != 31) {
        if (Rn == 31) {
            state->Regs[Rd] = state->SP + Op;
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

void sub(struct CompState* state, int instruction, char Rn, int Op) {
    
};

void subs(struct CompState* state, int instruction, char Rn, int Op) {
    sub(state, instruction, Rn, Op);
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

void movz(long *Regs, int instruction) {
    
};

void movn(long *Regs, int instruction) {
    
};

void movk(long *Regs, int instruction) {
    
};

void wideMove(struct CompState* state, int instruction) {
    
};

// Determines type of immediate instruction, arithmetic or wideMove.
void determineType(struct CompState* state, int instruction) {
    if (instruction & (BIT24)) {
        arithmetic(state, instruction);
    } else if (instruction & (BIT2523) == BIT2523) {
        wideMove(state, instruction);
    };
};

void main() {
    
}
