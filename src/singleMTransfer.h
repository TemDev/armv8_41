#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "emulate.h"


void accessMemory(void *location, char *mem, int address, size_t n, char t) {
	//assert(address % 4 == 0);

        assert(address < MEM_SIZE);
	if (t == 'w') {
        	memcpy(location, &mem[address], n);
	} else if (t =='r') {
		memcpy(&mem[address],location, n);
	}
}

void unsignedImmOffset(struct CompState *state, int inst, char *mem) {
	// check that U is one
	int L,U,I, Literal;
	long temp;
	int n; // number of bytes to be loaded 8 for 64bit and 4 for 32bit
	int xn, rt, xm;// register number
	long imm12, simm19;
	long simm9;
	long address = 0;
	printf("instruction: %x \n", inst);
	xn = (31) & (inst>>5);
	xm = (31) & (inst >> 16);
	rt = (31) & inst;
	//printf("imm12 before %x", imm12);
       	imm12 = (4095 & (inst >> 10));
	//printf("imm12 after %x",imm12);
	//printf("firt 22 bits of inst: %x", inst >> 10 );
	simm9 = (511 & (inst >> 12 ));
	printf("simm9 >> 8 :  %ld \n", simm9>>8);
	printf("mask :  %lx \n", 0xffffffffffffffff << 8 );	
	printf("simm9 :  %ld \n", simm9 | (0xffffffffffffffff << 8));
		

	if ((simm9 >> 8) == 1) {
		printf("simm9 >> 8 :  %ld \n", simm9>>8);
		simm9 = simm9 | (0xffffffffffffffff << 9);		
		printf("mask :  %lx \n", 0xffffffffffffffff << 9 );	
		printf("simm9 >> 8 :  %ld \n", simm9>>8);
		
	
	}	
	simm19 = ((1<<19)-1) & (inst >> 5);
	if ((simm19 >> 18) == 1) {
		simm19 = simm19 | (0xffffffffffffffff << 19);	
	}
	// can cause issues
	Literal = 1 & (inst>> 31);
	U = 1 & (inst>> 24);
	L = 1 & (inst>> 22);
	I = 1 & (inst>>11);
	printf("Literal:%d L:%d U:%d I:%d\n", Literal, L, U, I);

	if ((1 & (inst>>30)) == 0) {
		n = sizeof(int); //sf is 0 and 
	} else {
		n = sizeof(long);
	}
	printf("xn:%d rt:%d xm:%d\n", xn, rt, xm);
	
	if (Literal == 1) {
		if (U != 0) {
			printf("unsigned offset mode\n");
			printf("value of the xn register:%ld\n", (*state).Regs[xn]);
        		printf("immediate offset:%d\n", imm12);


			address = state -> Regs[xn] + (imm12 << 3);
		} else {
			if (I != 0) {
				printf("simm9 is %d\n", simm9);
				address = state -> Regs[xn] + simm9;
				state -> Regs[xn] = address;
				printf("pre index\naddress is %ld\n", address);
			} else {
				printf("post index\n");
				address = state -> Regs[xn];
				state -> Regs[xn] += simm9;
			}		
		}
		printf("address is %lx \n ", address);

		if (L == 0) {
			printf("store\n");
			accessMemory(&(*state).Regs[rt], mem, address, n, 'r');
			//memcpy(&mem[address], &(*state).Regs[rt], n);		
		} else {
			printf("load\n");
			accessMemory(&(*state).Regs[rt], mem, address, n, 'w');
				//memcpy(&(*state).Regs[rt], &mem[address],n);
			

			//printf("the memory to be copied %x\n", smth);
			//memcpy(&(*state).Regs[rt], &mem[address], n);
			//(*state).Regs[rt] = 0xabcdefaa;
			/*long *lp = &(state -> Regs[30]);
			printf("%ld this is the addresss\n", lp);
			printf("%ld value before\n", *lp);
			*lp = 0xabcdef;
			printf("%ld value before\n", *lp);*/

	}
	} else {

		printf("simm19 is %d\n", simm19);
		
		printf("literal\n");
		printf("address is %lx \n ", address);
		address = (long) state -> PC + (simm19 << 2);
		accessMemory(&(state -> Regs[rt]),mem,address, n, 'w');
	}

	printf("address where taken from%x \n", address);
}







