#include <stdbool.h>
#include <stdio.h>
#define NUM_GEN_REG 32
#define MEM_SIZE 0b100000000000000000000

extern struct Flags {
  bool N, Z, C, V;
};

extern struct CompState {
  long ZR;
  long PC;
  struct Flags PSTATE;
  long SP;
  long Regs[NUM_GEN_REG];
};
