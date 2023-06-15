#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "Aliases.h"


operand* insertReg(instruction_data *ins, operand elem, int32_t pos){
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

static operand RegisterZR(register_size size) {
    operand op;
    op.type = REGISTER;
    register_info reg;
    reg.type = SPECIAL;
    reg.size = size;
    op.value.register_operand = reg;
    return op;
}

instruction_data convert(instruction_data *inst) {

    instruction_data refined;
    operand mode;

    if (inst -> operands[0].value.register_operand.size == BIT_64) {
        mode = RegisterZR(BIT_64); // 64-bit mode -- xzr
    } else {
        mode = RegisterZR(BIT_32); // 32-bit mode -- wzr
    }

    // Check if the instruction is "cmp" and has two operands
    if (strcmp(inst->instruction_name, "cmp") == 0) {
        // Construct the "subs" instruction using the operands
        refined = (instruction_data){"subs", insertReg(inst, mode, 0), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "cmn") == 0) {
        // Construct the "adds" instruction using the operands
        refined = (instruction_data){"adds", insertReg(inst, mode, 0), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "neg") == 0) {
        // Construct the "sub" instruction using the operands
        refined = (instruction_data){"sub", insertReg(inst, mode, 1), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "negs") == 0) {
        // Construct the "subs" instruction using the operands
        refined = (instruction_data){"subs", insertReg(inst, mode, 1), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "tst") == 0) {
        // Construct the "ands" instruction using the operands
        refined = (instruction_data){"ands", insertReg(inst, mode, 0), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "mvn") == 0) {
        // Construct the "orn" instruction using the operands
        refined = (instruction_data){"orn", insertReg(inst, mode, 1), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "mov") == 0) {
        // Construct the "orr" instruction using the operands
        refined = (instruction_data){"orr", insertReg(inst, mode, 1), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "mul") == 0) {
        // Construct the "madd" instruction using the operands
        refined = (instruction_data){"madd", insertReg(inst, mode, 3), inst->no_operands + 1};

    } else if (strcmp(inst->instruction_name, "mneg") == 0) {
        // Construct the "msub" instruction using the operands
        refined = (instruction_data){"msub", insertReg(inst, mode, 3), inst->no_operands + 1};

    } else {
        refined = *inst; // in case the instruction is invalid or not a aliase
    }
    return refined;
}
