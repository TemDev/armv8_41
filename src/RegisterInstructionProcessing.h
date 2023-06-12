// Header file for RegisterInstructionProcessing.h

#include <stdint.h>
#include "emulate.h"

// Determines type of Register instruction - arithmetic, logical or multiplication.
extern void determineTypeRegister(CompState* state, int32_t instruction);
