// Useless but for reference is still here

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#define NUM_GEN_REG 32
#define MEM_SIZE 2097152

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

<<<<<<< HEAD
int B(char *c) {
	unsigned int sum = 0;
	int m = 1;
	int count = 0;
	for (int i = strlen(c) - 1; i > -1; i--) {
		if (c[i] != ' ') {
			if (c[i] == '1') {
				sum += m;
			} else if (c[i] != '0') {
				exit(1);
			}
			if (count < 32) {
				m = m << 1;
			}
			count++;
		
		}

	}
	return sum;
}
=======
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

// singleMTransfer
extern void unsignedImmOffset(struct CompState* state, int inst, char *mem);
extern int accessMemory(int address, char *mem, size_t n);

// branchInstructions
extern void branch(struct CompState* state, int instruction);
>>>>>>> 1178ca318990d4e25969299e2f406e7826841bdc
