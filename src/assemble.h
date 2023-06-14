
#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include<stdint.h>
#include<stdbool.h>

typedef enum { INSTRUCTION, DIRECTIVE, LABEL } line_type;

typedef enum { REGISTER, IMMEDIATE, ADDRESS, LABEL_NAME, SHIFT } operand_type;  // process address and literal types

typedef enum { GENERAL, SPECIAL } register_type;

typedef enum { SINGLETON, UNSIGNED, PRE, POST, REG, LOAD, REG_SHIFT } offset_type;

typedef enum { SP, ZR, PC } special_register_type;

typedef enum { BIT_64, BIT_32 } register_size;

typedef union { special_register_type special_register; int number; } register_id;

typedef struct { register_type type; register_id id; register_size size; } register_info;

typedef enum { LSL = 0, LSR = 1, ASR = 2, ROR = 3} shift_type;

typedef struct { shift_type shift; int shift_amount; } shift_info;

typedef struct 
{ 
  offset_type address_type;
  register_info operand1;
  union {register_info register_value; int32_t immediate_value;} operand2;
} address_info;


typedef union { register_info register_operand; int32_t immediate; char* label_name; address_info address1; shift_info shift_operand;} operand_value;

typedef struct { operand_type type; operand_value value; } operand; // add label vars

typedef struct {
  char *instruction_name;
  operand *operands;
  int no_operands;
} instruction_data;

// simplified directives as .int is the only one - below code could be used if there were more
// typdef struct { int64_t number } directive_arguments;
// typdef struct { char *name; directive_arguments *args; int args_length } directive_data;

typedef struct { char *name; int64_t arg; } directive_data;  // as .int takes exactly one integer as an argument

typedef union {
  instruction_data instruction;
  directive_data directive;
  char *label_name;
} line_contents;

typedef struct { line_type type; line_contents contents; } line_data;

#endif
