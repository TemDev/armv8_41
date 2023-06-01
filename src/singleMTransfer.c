#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Definitions.h"


int accessMemory(int address, char *mem, size_t n) {
	assert(address % 4 == 0);
        assert(address < MEM_SIZE);
        int p;
        memcpy(&p, &mem[address], n);
        return p;
}

void unsignedImmOffset(struct CompState* state, int inst, char *mem) {
	// check that U is one
	int L,U,I, Literal;
	long temp;
	int n; // number of bytes to be loaded 8 for 64bit and 4 for 32bit
	int xn, rt, xm;// register number
	unsigned int offset, simm19;
	long address;
	xn = (int) (0b11111 << 5) & inst;

	printf("%.32b", xn);
	if (Literal == 0) {
		if (U != 0) {
        		address = state -> Regs[xn] + (offset << 3);
		} else {
			int simm9;
			if (I != 0) {
				address = state -> Regs[xn] + simm9;
			} else {
				address = state -> Regs[xn];
				state -> Regs[xn] += simm9;
			}		
		}
	
		if (L == 0) {
			mem[address] = state -> Regs[xn];		
		} else {
			state -> Regs[xn] = accessMemory(address, *mem, n * sizeof (char));
		}
	} else {
	address = (long) state -> PC + simm19 << 2;
		state -> Regs[xn] = accessMemory(address, *mem, n * sizeof (char));
	}
}


