#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "Definitions.h"

#define BIT2826 335544320 // == 2 ^ 28 + 2 ^ 26
#define BIT313029282726 4227858432 // == 2 ^ 31 + 2 ^ 30 + 2 ^ 29 + 2 ^ 28 + 2 ^ 27 + 2 ^ 26
#define BIT31302826252019181716 3592355840 // == 2 ^ 31 + 2 ^ 30 + 2 ^ 28 + 2 ^ 26 + 2 ^ 25 + 2 ^ 20 + 2 ^ 19 + 2 ^ 18 + 2 ^ 17 + 2 ^ 16
#define NOTBIT98765 4294966303 // == bitwise_not(2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5)
#define BIT98765 992 // == 2 ^ 9 + 2 ^ 8 + 2 ^ 7 + 2 ^ 6 + 2 ^ 5
#define BIT31302928272625244 4278190096 // == 2 ^ 31 + 2 ^ 30 + 2 ^ 29 + 2 ^ 28 + 2 ^ 27 + 2 ^ 26 + 2 ^ 25 + 2 ^ 24 + 2 ^ 4
#define BIT302826 1409286144 // == 2 ^ 30 + 2 ^ 28 + 2 ^ 26
#define BIT26MASK 67108863 // == 2 ^ 26 - 1
#define BIT235MASK 16777184 // == 2 ^ 24 - 2 ^ 5
#define BIT4MASK 15 // == 2 ^ 4 - 1

void unconditional(struct CompState* state, long simm26) {
    state->PC = simm26 * 4;
};

void registerBranch(struct CompState* state, char Rn) {
    if (Rn != 31) {
        state->PC = state->Regs[Rn];
    };
};

void conditional(struct CompState* state, long offset, char cond) {
    switch (cond) {
        case 0:
        if (state->PSTATE.Z == 1) {
            state->PC = offset;
        };
        break;
        case 1:
        if (state->PSTATE.Z == 0) {
            state->PC = offset;
        };
        break;
        case 6:
        if (state->PSTATE.N == state->PSTATE.V) {
            state->PC = offset;
        };
        break;
        case 7:
        if (state->PSTATE.N != state->PSTATE.V) {
            state->PC = offset;
        };
        break;
        case 8:
        if (state->PSTATE.Z == 0 && state->PSTATE.N == state->PSTATE.V) {
            state->PC = offset;
        };
        break;
        case 9:
        if (!(state->PSTATE.Z == 0 && state->PSTATE.N == state->PSTATE.V)) {
            state->PC = offset;
        };
        break;
        case 10:
        if (state->PSTATE.N == 1 || state->PSTATE.Z == 1 || state->PSTATE.C == 1
         || state-> PSTATE.V == 1) {
            state->PC = offset;
        };
    }
};

void branch(struct CompState* state, int instruction) {
    if ((instruction & BIT313029282726) == BIT2826) {
        long simm26 = instruction & BIT26MASK;
        unconditional(state, simm26);
    } else if ((instruction & NOTBIT98765) == BIT31302826252019181716) {
        char Rn = (instruction & BIT98765) >> 5;
        registerBranch(state, Rn);
    } else if ((instruction & BIT31302928272625244) == BIT302826) {
        long simm19 = instruction & BIT235MASK;
        char cond = instruction & BIT4MASK;
        conditional(state, simm19 * 4, cond);
    };
};

void mainBranch() {
    
};