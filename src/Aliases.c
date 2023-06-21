// Handles the aliases functions and converts them to functions that are already encoded

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "Aliases.h"

//Function to correctly place the zero-register into the instruction created 
// after dealing with aliases
typedef struct{
    char* alias;
    char* name;
    char index;
} aliasT;
#define NUM_ALIASES 9
aliasT aliases [NUM_ALIASES] = {{"cmp", "subs", 0},  {"cmn", "adds", 0}, {"neg", "sub", 1},
                                {"negs", "subs", 1}, {"tst", "ands", 0}, {"mvn", "orn", 1},
                                {"mov", "orr", 1},   {"mul", "madd", 3}, {"mneg", "msub", 3}};


operand* insertReg(instruction_data *ins, operand elem, int pos){
    operand *ops = calloc(ins -> no_operands + 1, sizeof(operand));
    memcpy(ops, ins -> operands, ins -> no_operands * sizeof(operand));
    for (int32_t i = ins -> no_operands - 1 ; i > pos - 1 ; i--)
    {
        if ( i >= pos) {
        ops[i + 1] = ops[i] ;
        }
    }
    ops[pos] = elem;
    return ops;
}

// Function to correctly create the zero-register of required size

static operand RegisterZR(register_size size) {
    operand op;
    op.type = REGISTER;
    register_info reg;
    reg.type = SPECIAL;
    reg.size = size;
    op.value.register_operand = reg;
    return op;
}

// Converts instruction alias to regular instruction

instruction_data convert(instruction_data *inst) {

    operand mode;

    if (inst -> operands[0].value.register_operand.size == BIT_64) {
        mode = RegisterZR(BIT_64); // 64-bit mode -- xzr
    } else {
        mode = RegisterZR(BIT_32); // 32-bit mode -- wzr
    }

    for (int32_t i = 0; i < NUM_ALIASES; i++) {
        if (strcmp(inst->instruction_name, aliases[i].alias) == 0) {
            // if it is an aliases it returns an original instruction that makes into one of the known ones
        return (instruction_data){aliases[i].name, insertReg(inst, mode, aliases[i].index), inst->no_operands + 1};
    } 
    }
    // it it is not in the alisaes then it is a regular instruction and does not require processing
    return *inst;
}
