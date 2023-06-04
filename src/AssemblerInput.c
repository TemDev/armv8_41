#include <stdio.h>

typedef enum { INSTRUCTION, DIRECTIVE, LABEL } line_type;

typedef enum { REGISTER, IMMEDIATE } operand_type;

typedef union { char *register, int64_t immediate } operand_value;

typedef struct { operand_type type, operand_value value; } operand; 

typedef struct {
    char *instruction_name,
    operand *operands,
    int no_operands;
} instruction_data;

typedef union {
    instruction_data instruction,
    char *directive_name,
    char *label_name;
} line_contents;

typedef struct { line_type type, line_contents contents; } line_data;


