
#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include<stdint.h>
#include<stdbool.h>
#include <assert.h>

// In this file we create all the datatypes we use afterwarde to process the different
// components of instruction we have received

typedef enum { INSTRUCTION, DIRECTIVE, LABEL } line_type; // types pf instuction in a line of input that we can encounter

typedef enum { REGISTER, IMMEDIATE, ADDRESS, LABEL_NAME, SHIFT } operand_type;  // types of operands

typedef enum { GENERAL, SPECIAL } register_type; // types of registers

typedef enum {UNSIGNED, PRE, POST, LOAD, REG_SHIFT } offset_type; // types of offsets

typedef enum { SP, ZR, PC } special_register_type; // types of special registers

typedef enum { BIT_64, BIT_32 } register_size; // register sizes

typedef enum { LSL = 0, LSR = 1, ASR = 2, ROR = 3} shift_type; // types of shifts
 
typedef union { special_register_type special_register; int number; } register_id; // registers id that could be encountered

typedef struct 
{ 
  register_type type;
  register_id id;
  register_size size; 
} register_info; // information about the register


typedef struct 
{ 
  shift_type shift;
  int shift_amount; 
} shift_info; // information that shifts hold

typedef struct 
{ 
  offset_type address_type;
  register_info operand1;
  union {register_info register_value; int32_t immediate_value;} operand2;
} address_info; // information that an address holds


typedef union { register_info register_operand; int32_t immediate;
 char* label_name; address_info address1; shift_info shift_operand;} operand_value; // types of operand values

typedef struct 
{ 
  operand_type type;
  operand_value value;
} operand; // operand information

typedef struct {
  char *instruction_name;
  operand *operands;
  int no_operands;
} instruction_data; // instruction data information


typedef struct 
{ 
  char *name;
  int32_t arg;
} directive_data;  // directives information

typedef union {
  instruction_data instruction;
  directive_data directive;
  char *label_name;
} line_contents; // lines of contents information

typedef struct 
{ 
  line_type type;
  line_contents contents;
} line_data; // information given in a line

#endif
