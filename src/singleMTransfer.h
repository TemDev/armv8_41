#include "emulate.h"

extern void unsignedImmOffset(struct CompState* state, int inst, char *mem);
extern int accessMemory(int address, char *mem, size_t n);