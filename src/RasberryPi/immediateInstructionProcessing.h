// Header file for immediateInstructionProcessing.c

#include "rasberryPi.h"

// Determines type of immediate instruction, arithmetic or wideMove.
extern void determineTypeImmediate(struct CompState* state, int32_t instruction);

// Function used to add contentsof register Rn and an operand Op and load into register. Does not set flags.
extern void add(struct CompState* state, int32_t instruction, unsigned char Rn, int64_t Op);

// Function used to add contentsof register Rn and an operand Op and load into register. Sets flags. 
extern void adds(struct CompState* state, int32_t instruction, unsigned char Rn, int64_t Op, char subOrNot);
