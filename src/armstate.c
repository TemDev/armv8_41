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

/* 
 * this function prints the contents of 
 * the registers to the file specified
 * file is not closed 
 */
void printContentsTo(FILE *fp, struct CompState* state) {
	fprintf(fp, "%5s : %.16x \n", "PC",  state->PC);
	fprintf(fp, "%5s : %.16x \n", "ZR", state->ZR);
	fprintf(fp, "%5s : %.16x \n", "N", state->PSTATE.N);
	fprintf(fp, "%5s : %.16x \n", "Z", state->PSTATE.Z);
	fprintf(fp, "%5s : %.16x \n", "C", state->PSTATE.C);
	fprintf(fp, "%5s : %.16x \n", "V", state->PSTATE.V);
	fprintf(fp, "%5s : %.16x \n", "SP",state->SP);
	for (int i = 0; i<NUM_GEN_REG; i++) {
		fprintf(fp, "X%d: %.16x \n", i, state -> Regs[i]);

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
	printContentsTo(fp, &state);
	fclose(fp);

}	