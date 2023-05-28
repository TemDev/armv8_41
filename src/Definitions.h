#include <stdbool.h>
#include <stdin.h>
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
