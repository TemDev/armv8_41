#include <stdio.h>
#define NUM_GEN_REG 32

struct Flags {
	char N, Z, C, V;
};

struct CompState {
	long ZR;
	long PC;
	struct Flags PSTATE;
	long SP;
	long Regs[NUM_GEN_REG];
};

void initial(struct CompState* statep) {
	statep->ZR = 0;
	statep->PC  = 0;
	statep->PSTATE.N = 0;
	statep->PSTATE.Z = 0;
	statep->PSTATE.C = 0;
	statep->PSTATE.V = 0;
	statep->SP = 0;
	for (int i = 0; i < NUM_GEN_REG; i++) {
		statep->Regs[i] = 0;
	}
}

void incrementPC(struct CompState* state) {
	state->PC += 4;
}


// mutates output to have the correct letters for set flags
void formatPSTATE(struct CompState* state, char *output) {
        if (state->PSTATE.N == 1) output[0] = 'N';
        if (state->PSTATE.Z == 1) output[1] = 'Z';
        if (state->PSTATE.C == 1) output[2] = 'C';
        if (state->PSTATE.V == 1) output[3] = 'V';
}



/* 
 * this function prints the contents of 
 * the registers to the file specified
 * file is not closed 
 */
void printContentsTo(FILE *fp, struct CompState* state) {
	fprintf(fp, "%5s : %.16ld \n", "PC",  state->PC);
	fprintf(fp, "%5s : %.16ld \n", "ZR", state->ZR);
        char pstateOutput[] = "----";
        formatPSTATE(state, pstateOutput);
	fprintf(fp, "%5s : %s \n", "PSTATE", pstateOutput);

	for (int i = 0; i<NUM_GEN_REG; i++) {
		fprintf(fp, "X%d: %.16ld \n", i, state -> Regs[i]);

	}
}
void changeRegister(long* src, long* dest) {
	*dest = (long int) src;
}

void loadRegister(long* dest, long value) {
	*dest = value;
}

int main(void) {
	struct CompState state;
	FILE *fp = fopen("output.out","w");
	initial(&state);
	printContentsTo(fp, &state);
	fclose(fp);
	fp = fopen("output2.out", "w");
	incrementPC(&state);
	state.PSTATE.C = 1;  // doesn't make logical sense, just for testing
	printContentsTo(fp, &state);
	fclose(fp);

}	
