// Header file for the main emulate.c file

// Will only run once regardless of how many includes which prevents multiple definition issues and so on...
#ifndef EMULATE_H
#define EMULATE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define NUM_GEN_REG 32
#define MEM_SIZE 2097152

// Type definition for the structure that contains all the flags
typedef struct Flags {
  bool N, Z, C, V;
} Flags;

// Type definition for the structure that contains the states of all registers and also the state of the flag
typedef struct CompState {
  int64_t ZR;
  int64_t PC;
  Flags PSTATE;
  int64_t SP;
  int64_t Regs[NUM_GEN_REG];
} CompState;
#endif
