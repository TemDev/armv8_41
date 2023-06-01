// Will only run once regardless of how many includes which prevents multiple definition issues and so on...

#ifndef EMULATE_H
#define EMULATE_H

#include <stdbool.h>

#define NUM_GEN_REG 32

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

#endif