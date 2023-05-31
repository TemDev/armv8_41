#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include Definitions.h
#include bitwise.c
#define BIT24 16777216 // == 2 ^ 24, MASK
#define BIT2523 41943040 // == 2 ^ 25 + 2 ^ 23, MASK
#define BITsf 2147483648 // == 2 ^ 31, MASK
#define BITrd 31 // == 2 ^ 4 + 2 ^ 3 + 2 ^ 2 + 2 ^ 1 + 2 ^ 0, MASK
#define BIT32MASK 4294967295 // == 2 ^ 32 - 1, MASK
#define BIT64MASK 18446744073709551615 // == 2 ^ 64 - 1, MASK
#define BIT64 18446744073709551616 // == 2 ^ 64, MASK
#define REGISTER31 31
#define BIT63 9223372036854775808 // == 2 ^ 63, MASK
#define BIT31 2147483648 // == 2 ^ 31, MASK
#define BIT32 4294967296 // == 2 ^ 32, MASK

void add(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == REGISTER31) {
            result = state->SP + Op;
        } else {
            result = state->Regs[Rn] + Op;
        };
        
        if (Rd == REGISTER31) {
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
        if (Rn == REGISTER31) {
            result = (state->SP & BIT32MASK) + Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) + Op;
        };
        
        if (Rd == REGISTER31) {
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

void adds(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == REGISTER31) {
            result = state->ZR + Op;
        } else {
            result = state->Regs[Rn] + Op;
        };
        
        //Set Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        if (result > 0) {
            state->PSTATE.C = result & BIT64 ? true : false;
        } else {
            state->PSTATE.C = false;
        };
        state->PSTATE.V = state->PSTATE.C;
        if (!(Rd == REGISTER31)) {
            if (result < 0) {
                state->Regs[Rd] = result;
            } else {
                state->Regs[Rd] = result & BIT64MASK;
            };
        };
        
    } else {
        long result;
        if (Rn == REGISTER31) {
            result = (state->ZR & BIT32MASK) + Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) + Op;
        };
        
        //Set Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        if (result > 0) {
            state->PSTATE.C = result & BIT32 ? true : false;
        } else {
            state->PSTATE.C = false;
        };
        state->PSTATE.V = state->PSTATE.C;
        
        if (!(Rd == REGISTER31)) {
            if (result < 0) {
                int res = result;
                state->Regs[Rd] = res;
            } else {
                state->Regs[Rd] = result & BIT32MASK;
            };
        };
    };
};

void sub(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == REGISTER31) {
            result = state->SP - Op;
        } else {
            result = state->Regs[Rn] - Op;
        };
        
        if (Rd == REGISTER31) {
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
        if (Rn == REGISTER31) {
            result = (state->SP & BIT32MASK) - Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) - Op;
        };
        
        if (Rd == REGISTER31) {
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

void subs(struct CompState* state, int instruction, char Rn, int Op) {
    char Rd = BITrd & instruction;
    
    if (BITsf & instruction) {
        long long result;
        if (Rn == REGISTER31) {
            result = state->SP - Op;
        } else {
            result = state->Regs[Rn] - Op;
        };
        
        //Set Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        if (result > 0) {
            state->PSTATE.C = result & BIT64 ? true : false;
        } else {
            if (result < -BIT63) {
                state->PSTATE.C = true;
            } else {
                state->PSTATE.C = false;
            };
        };
        state->PSTATE.V = state->PSTATE.C;
        if (!(Rd == REGISTER31)) {
            if (result < 0) {
                state->Regs[Rd] = result;
            } else {
                state->Regs[Rd] = result & BIT64MASK;
            };
        };
        
    } else {
        long result;
        if (Rn == REGISTER31) {
            result = (state->SP & BIT32MASK) - Op;
        } else {
            result = (state->Regs[Rn] & BIT32MASK) - Op;
        };
        
        //Set Flags
        state->PSTATE.N = result < 0 ? true : false;
        state->PSTATE.Z = result == 0 ? true : false;
        if (result > 0) {
            state->PSTATE.C = result & BIT32 ? true : false;
        } else {
            if (result < -BIT31) {
                state->PSTATE.C = true;
            } else {
                state->PSTATE.C = false;
            };
        };
        state->PSTATE.V = state->PSTATE.C;
        
        if (!(Rd == REGISTER31)) {
            if (result < 0) {
                int res = result;
                state->Regs[Rd] = res;
            } else {
                state->Regs[Rd] = result & BIT32MASK;
            };
        };
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

void movn(struct CompState* state, int instruction, long Op) {
    char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
        state->Regs[Rd] = ~Op;
    } else { // 32 bit mode.
        int op = Op;
        state->Regs[Rd] = ~op;
    };
};

void movz(struct CompState* state, int instruction, long Op) {
    char Rd = BITrd & instruction;
    if (BITsf & instruction) { // 64 bit mode.
        state->Regs[Rd] = Op;
    } else {
        int op = Op;
        state->Regs[Rd] = op;
    };
};

void movk(struct CompState* state, int instruction, int imm16, char hw) {
    char Rd = BITrd & instruction;
    const long MASK = imm16 * pow(2, hw * 16);
    state->Regs[Rd] = state->Regs[Rd] ^ MASK;
};

void wideMove(struct CompState* state, int instruction) {
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
void determineType(struct CompState* state, int instruction) {
    if (instruction & (BIT24)) {
        arithmetic(state, instruction);
    } else if (instruction & (BIT2523) == BIT2523) {
        wideMove(state, instruction);
    };
};

void main() {
    
};
