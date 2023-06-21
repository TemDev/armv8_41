// Header for the singleMTransfer.c file

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "rasberryPi.h"

// Function used to access a memory location
extern void accessMemory(void *location, char *mem, int32_t address, size_t n, char t);

// Function used to determine and execute the type of single Data Transfer instruction
extern void unsignedImmOffset(CompState *state, int32_t inst, char *mem);
