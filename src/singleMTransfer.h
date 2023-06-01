#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Definitions.h"


void accessMemory(void *location, int address, char *mem, size_t n) {
	assert(address % 4 == 0);
        assert(address < MEM_SIZE);
        memcpy(location, &mem[address], n);
}

void unsignedImmOffset(struct CompState *state, int inst, char *mem) {
	// check that U is one
	int L,U,I, Literal;
	long temp;
	int n; // number of bytes to be loaded 8 for 64bit and 4 for 32bit
	int xn, rt, xm;// register number
	unsigned long imm12, simm19;
	long simm9;
	long address = 0;
	printf("%d \n", inst);
	xn = (B("11111")) & (inst>>5);
	xm = (B("11111")) & (inst >> 16);
	rt = (B("11111")) & inst;
	imm12 = ((1 << 13) - 1) & (inst >> 10);
	simm9 = (B("111111111") & (inst >> 12 ));
	Literal = 1 & (inst>> 29);
	U = 1 & (inst>> 24);
	L = 1 & (inst>> 22);
	I = 1 & (inst>>11);
	printf("Literal:%d L:%d U:%d I:%d\n", Literal, U, L, I);

	if (1 & (inst>>30) == 0) {
		n = 4; //sf is 0 and 
	} else {
		n = 8;
	}
	printf("%d \n", n);
	printf("xn:%d rt:%d xm:%d\n", xn, rt, xm);
	
	if (Literal == 1) {
		if (U != 0) {
			printf("unsigned offset mode\n");
        		address = state -> Regs[xn] + (imm12 << 3);
		} else {
			if (I != 0) {
				printf("simm9 is %d\n", simm9);
				address = state -> Regs[xn] + simm9;
				printf("pre index\naddress is %ld\n", address);
			} else {
				printf("post index\n");
				address = state -> Regs[xn];
				state -> Regs[xn] += simm9;
			}		
		}
	
		if (L == 0) {
			printf("store\n");
			memcpy(&mem[address], &(state -> Regs[xn]), n);		
		} else {
			printf("load\n");
			accessMemory(&(state -> Regs[rt]),address, mem, n * sizeof (char));
			long *lp = &(state -> Regs[30]);
			printf("%ld this is the addresss\n", lp);
			printf("%ld value before\n", *lp);
			*lp = 0xabcdef;
			printf("%ld value before\n", *lp);

		}
	} else {
		printf("literal\n");
		address = (long) state -> PC + simm19 << 2;
		accessMemory(&((*state).Regs[xn]),address, mem, n * sizeof (char));
	}

	printf("%d \n", address);
}


