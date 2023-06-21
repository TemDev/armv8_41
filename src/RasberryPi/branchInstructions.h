// Header file for branch instructions.

#include <stdint.h>
#include "rasberryPi.h"

// Function that determines what type of branch is being requested
extern void branch(CompState* state, int32_t instruction);
