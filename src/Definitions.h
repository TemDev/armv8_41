#include <stdbool.h>
#include <stdio.h>
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
