#include <stdio.h>
#define NUM_GEN_REG 32
struct CompState {
	long ZR;
	long PC;
	long PSTATE;
	long SP;
	long Regs[NUM_GEN_REG];
};

void initial(struct CompState* statep) {
	statep->ZR = 0;
	statep->PC  = 0;
	statep->PSTATE = 0;
	statep->SP = 0;
	for (int i = 0; i < NUM_GEN_REG; i++) {
		statep->Regs[i] = 0;
	}
}

void incrementPC(struct CompState* state) {
	state->PC += 1;
}

/* 
 * this function prints the contents of 
 * the registers to the file specified
 * file is not closed 
 */
void printContentsTo(FILE *fp, struct CompState* state) {
	fprintf(fp, "%5s : %.16x \n", "PC",  state->PC);
	fprintf(fp, "%5s : %.16x \n", "ZR", state->ZR);
	fprintf(fp, "%5s : %.16x \n", "PSTATE", state->PSTATE);
	fprintf(fp, "%5s : %.16x \n", "SP",state->SP);
	for (int i = 0; i<NUM_GEN_REG; i++) {
		fprintf(fp, "X%d: %.16x \n", i, state -> Regs[i]);

	}
}
void changeRegister(long* src, long* dest) {
	*dest = src;
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
