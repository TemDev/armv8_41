// Header file for AssemblerDecoder.c
#include <stdint.h>
#include "assemble.h"

// Function that determines a DIRECTIVE, INSTRUCTION or LABEL is presented
extern int32_t decodeline(line_data line);

typedef int32_t (*instructionMaker) (instruction_data*,char); // Function pointer that returns an int from an instruction_data and char

typedef struct {
    char* key;
    instructionMaker function;
    char opcode;
} instToFunction; // struct that holds a key, a function and an opcode
