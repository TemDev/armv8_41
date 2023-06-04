// Will only run once regardless of how many includes which prevents multiple definition issues and so on...

#ifndef EMULATE_H
#define EMULATE_H

#include <stdbool.h>
#include <string.h>
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

static int B(char *c) {
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

#endif
