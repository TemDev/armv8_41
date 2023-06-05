#include "emulate.h"

extern void determineTypeImmediate(struct CompState* state, int instruction);
extern void add(struct CompState* state, int instruction, char Rn, long Op);
extern void adds(struct CompState* state, int instruction, char Rn, long Op);
