#include <stdio.h>
#include <stdbool.h>
#define NUM_GEN_REG 32

struct Flags {
  bool N, Z, C, V;
};

//functions for changing the values of flags for encapsulation purposes
void changeN (bool* N) {
  *N = false;
};

void changeZ (bool* Z) {
  *Z = false;
}

void changeC (bool* C) {
  *C = false;
}

void changeV (bool* V) {
  *V = false;
}

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
  statep->PSTATE.N = false;
  statep->PSTATE.Z = false;
  statep->PSTATE.C = false;
  statep->PSTATE.V = false;
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
  if (state->PSTATE.N) output[0] = 'N';
  if (state->PSTATE.Z) output[1] = 'Z';
  if (state->PSTATE.C) output[2] = 'C';
  if (state->PSTATE.V) output[3] = 'V';
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
  state.PSTATE.C = true;  // doesn't make logical sense, just for testing
  printContentsTo(fp, &state);
  fclose(fp);

}
