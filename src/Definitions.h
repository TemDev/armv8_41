#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
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

// Bitwise
extern int64_t lsl_64(int64_t operand, int shift_amount);
extern int64_t lsr_64(int64_t operand, int shift_amount);
extern int64_t asr_64(int64_t operand, int shift_amount);
extern int64_t ror_64(int64_t operand, int shift_amount);
extern int32_t lsl_32(int64_t operand, int shift_amount);
extern int32_t lsr_32(int64_t operand, int shift_amount);
extern int32_t asr_32(int64_t operand, int shift_amount);
extern int32_t ror_32(int64_t operand, int shift_amount);

// RegisterInstructionProcessing
extern void determineTypeRegister(struct CompState* state, int instruction);

// ImmediateInstructionProcessing
extern void determineTypeImmediate(struct CompState* state, int instruction);
