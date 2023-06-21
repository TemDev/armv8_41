// Handles Single Data Transfer instructions by determining their type and updating appropriate memory

#include <assert.h>
#include "singleMTransfer.h"
#include "rasberryPi.h"

// Function used to access a memory location
void accessMemory(void *location, char *mem, int32_t address, size_t n, char t) {
    //assert(address % 4 == 0);

    assert(address < MEM_SIZE);
    if (t == 'w') {
        memcpy(location, &mem[address], n);
    } else if (t =='r') {
	memcpy(&mem[address],location, n);
    }
}

// Function used to determine and execute the type of single Data Transfer instruction
void unsignedImmOffset(CompState *state, int32_t inst, char *mem) {
    // check that U is one
    int32_t L,U,I, Literal, R;
    //int64_t temp;
    int32_t n; // number of bytes to be loaded 8 for 64bit and 4 for 32bit
    int32_t xn, rt, xm;// register number
    int64_t imm12, simm19;
    int64_t simm9;
    int64_t address = 0;
    xn = (31) & (inst>>5);
    xm = (31) & (inst >> 16);
    rt = (31) & inst;
    //printf("imm12 before %x", imm12);
    imm12 = (4095 & (inst >> 10));
    //printf("imm12 after %x",imm12);
    //printf("firt 22 bits of inst: %x", inst >> 10 );
    simm9 = (511 & (inst >> 12 ));

    if ((simm9 >> 8) == 1) {
	simm9 = simm9 | (0xffffffffffffffff << 9);
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
    R = 1 & (inst >> 21);
    
    if ((1 & (inst>>30)) == 0) {
	n = sizeof(int32_t); //sf is 0 and 
    } else {
	n = sizeof(int64_t);
    }

    if (Literal == 1) {
        if (U != 0) {
	    address = state -> Regs[xn] + (imm12 << 3);
	} else {	
	    if ((R == 1)) {
	        //register offset
		address = state -> Regs[xn] + state -> Regs[xm];
	    } else if (I != 0) {
	        address = state -> Regs[xn] + simm9;
	        state -> Regs[xn] = address;;
	    } else {
		address = state -> Regs[xn];
	        state -> Regs[xn] += simm9;
	    }		
        }
	if (L == 0) {
	    accessMemory(&(*state).Regs[rt], mem, address, n, 'r');
	    //memcpy(&mem[address], &(*state).Regs[rt], n);		
	} else {
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
	address = (int64_t) state -> PC + (simm19 << 2);
	accessMemory(&(state -> Regs[rt]),mem,address, n, 'w');
    }
}
