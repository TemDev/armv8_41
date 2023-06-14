#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "AssemblerInput.h"
// #include "assemble.h"


#ifndef ASSEMBLE_H
#define ASSEMBLE_H

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
  shift_info shift_operand;
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



operand RegisterN(char n) {
    operand op;
    op.type = REGISTER;
    register_info reg;
    reg.type = GENERAL;
    reg.size = BIT_64;
    reg.id.number = n;
    op.value.register_operand = reg;
    return op;
}

operand RegisterNsize(char n, char s) {
    operand op;
    op.type = REGISTER;
    register_info reg;
    reg.type = GENERAL;
    reg.size = (s == 'x')? BIT_64: BIT_32;
    reg.id.number = n;
    op.value.register_operand = reg;
    return op;
}
operand RegisterZR(char c) {
    operand op;
    op.type = REGISTER;
    register_info reg;
    reg.type = SPECIAL;
    reg.size = BIT_64;
    register_id id;
    id.special_register = ZR;
    op.value.register_operand = reg;
    return op;
}
//
//operand RegisterSpecial(special_register_type type, s) {
//    operand op;
//    op.type = REGISTER;
//    register_info reg;
//    reg.type = SPECIAL;
//    reg.size = BIT_64;
//    register_id id;
//    id.special_register = ZR;
//    op.value.register_operand = reg;
//    return op;
//}
// operand shiftmake(shift_type type, int amount) {
//     operand op;
//     op.type = SHIFT;
//     shift_info shiftop;
//     shiftop.shift = type;
//     shiftop.shift_amount = amount;
//     op.value.shift_operand = shiftop;

//     return op;
// }

operand immediateMake(char* imm_str) {
    operand op;
    op.type = IMMEDIATE;
    // checks if hex and converts
    op.value.immediate = (imm_str[1] == 'x') ? strtol(imm_str, NULL, 16) : atoi(imm_str);
    return op;
}
// built in func exists strchr BUT don't think it works as can't check if '\0' ?
int char_in_str(char val, char *str, int str_len) {
    for(int i = 0; i < str_len; i++) {
        if(val == str[i]) return 1;  // if char is in str
    }
    return 0;
}


int str_in_str_arr(char *str, char **str_arr, int str_arr_len) {
    for(int i = 0; i < str_arr_len; i++) {
        if(strlen(str) == strlen(str_arr[i])) {
            if (strcmp(str, str_arr[i]) == 0) return 1;  // if str is in str_arr
        }
    }
    return 0;
}

static void remove_space_from_operand(char *operand_text) {
    while(operand_text[0] == ' ') operand_text++;
}


line_type get_line_type(char *file_line) {
    int length = strlen(file_line);
    if(file_line[0] == '.') return DIRECTIVE;
    if(file_line[length-1] == ':') return LABEL;
    return INSTRUCTION;
}

int is_special_register(char* operand_text) {
#define SRN_LEN 5
    char *SPECIAL_REGISTER_NAMES[] = {"sp", "wsp", "xzr", "wzr", "PC"};
    if(str_in_str_arr(operand_text, SPECIAL_REGISTER_NAMES, SRN_LEN)) return 1;
    return 0;
}

int is_general_register(char* operand_text) {
    if(operand_text[0] == 'x' || operand_text[0] == 'w') {
        char *str;
        long num = strtol(operand_text + 1, &str, 10);
        if (*str == '\0') {// changed  from *str != num && *str == '\0'
        // because x0 is not recognised as general register
            if (0 <= num && num <= 31) return 1;
        }
    }
    return 0;
}

int is_shift_operation(char* operand_text) {
#define SN_LEN 4
    char *SHIFT_NAMES[] = {"lsl", "lsr", "asl", "ror"};
    char shift_chars[4];
    strncpy(shift_chars, operand_text, 3);
    if(str_in_str_arr(shift_chars, SHIFT_NAMES, SN_LEN) && operand_text[4] == '#') return 1;
    return 0;
}

int is_address(char *operand_text) {
    return (operand_text[0] == '[') ? 1 : 0;
}

shift_type get_shift_type(char* shift) {
    if (strcmp("lsl", shift) == 0) return LSL;
    else if (strcmp("lsr", shift) == 0) return LSR;
    else if (strcmp("asr", shift) == 0) return ASR;
    else if (strcmp("ror", shift) == 0) return ROR;
    else {
        perror("Unknown type of shift");
    }
}

operand process_special_register_operand(char* operand_text) {
    operand ret_op;
    ret_op.type = REGISTER;
    ret_op.value.register_operand.type = SPECIAL;
    if (strcmp(operand_text + 1,"p") == 0) {
        ret_op.value.register_operand.id.special_register = SP;
        ret_op.value.register_operand.size = BIT_64;
    } else if (strcmp(operand_text + 1,"sp") == 0) {
        ret_op.value.register_operand.id.special_register = SP;
        ret_op.value.register_operand.size = BIT_32;
    } else if (strcmp(operand_text + 1,"zr") == 0) {
        ret_op.value.register_operand.id.special_register = ZR;
        ret_op.value.register_operand.size = (operand_text[0] == 'x') ? BIT_64 : BIT_32;
    } else { // PC registers
        ret_op.value.register_operand.id.special_register = PC;
        ret_op.value.register_operand.size = BIT_64;
    }
    return ret_op;
}

shift_info process_shift_operand(char *operand_text) {
    shift_info ret_shift;
    // ret_op.type = SHIFT;  // set shift_operand.shift, shift_operand.amount type and amount
    char shift_chars[4];
    strncpy(shift_chars, operand_text, 3);
    
    if (strcmp(shift_chars, "lsl") == 0) ret_shift.shift = LSL;
    else if (strcmp(shift_chars, "lsr") == 0) ret_shift.shift = LSR;
    else if (strcmp(shift_chars, "asr") == 0) ret_shift.shift = ASR;
    else ret_shift.shift = ROR;  // must be ROR
    
    ret_shift.shift_amount = atoi(operand_text+5);
    return ret_shift;
}

void find_address_operand_vars(operand *addr_operand, char *operand_text) {
    char *reg = strtok(operand_text, ",");
    char *var2 = strtok(NULL, ",");
    if( var2 != NULL ) {
        remove_space_from_operand(var2);
        if( is_general_register(reg + 1) ) {
            addr_operand->value.address1.operand1 
              = RegisterNsize(atoi(reg + 2), reg[1]).value.register_operand;
        }
        else {
            addr_operand->value.address1.operand1
              = process_special_register_operand(reg + 1).value.register_operand;
        }
        char *var3 = strtok(NULL, ",");
        if( var3 != NULL ) {
            remove_space_from_operand(var3);
            var3[strlen(var3) - 1] = '\0';
            addr_operand->value.address1.shift_operand = process_shift_operand(var3);
            addr_operand->value.address1.address_type = REG_SHIFT;
            if( is_general_register(var2) ) {
                addr_operand->value.address1.operand2.register_value 
                  = RegisterNsize(atoi(var2 + 1), *var2).value.register_operand;
            }
            else {  // must be special register
                operand sp_reg_operand;
                addr_operand->value.address1.operand2.register_value
                  = process_special_register_operand(var2).value.register_operand;
            }
        } else {
            if(var2[strlen(var2) - 1] == '!') var2[strlen(var2) - 2] = '\0';
            var2[strlen(var2) - 1] = '\0';
            if( is_general_register(var2) ) {
                addr_operand->value.address1.address_type = REG;
                addr_operand->value.address1.operand2.register_value 
                  = RegisterNsize(atoi(var2 + 1), *var2).value.register_operand;
            }
            else if( is_special_register(var2) ) {
                addr_operand->value.address1.address_type = REG;
                addr_operand->value.address1.operand2.register_value
                  = process_special_register_operand(var2).value.register_operand;
            }
            else {
                addr_operand->value.address1.address_type = UNSIGNED;
                addr_operand->value.address1.operand2.immediate_value = atoi(var2 + 1);
            } 
        }
    } else {
        reg = strtok(operand_text, "]") + 1;
        if( is_general_register(reg) ) {
            addr_operand->value.address1.operand1 
              = RegisterNsize(atoi(reg + 1), reg[0]).value.register_operand;
        } else {  // is special register
            addr_operand->value.address1.operand1
              = process_special_register_operand(reg + 1).value.register_operand;
        }
        char *var2 = strtok(NULL, "]");
        if( var2 != NULL ) {
            char *imm_val = strtok(operand_text, "#");
            addr_operand->value.address1.operand2.immediate_value = atoi(imm_val);
            addr_operand->value.address1.address_type = POST;
        } else {
            addr_operand->value.address1.address_type = SINGLETON;
        }
    }
    if( operand_text[strlen(operand_text) - 1] == '!' ) addr_operand->value.address1.address_type = PRE;
}


operand process_operand(char* operand_text) {
    operand ret_operand;
    if(operand_text[0] == '#') {
        ret_operand = immediateMake(operand_text + 1);
        // assumes immediate value is valid, else is set to 0
    } else if(is_special_register(operand_text)) {
        ret_operand = process_special_register_operand(operand_text);
    } else if(is_general_register(operand_text)) {
        ret_operand = RegisterNsize(atoi(operand_text + 1), *operand_text);
    } else if(is_shift_operation(operand_text)) {
        ret_operand.type = SHIFT;
        ret_operand.value.shift_operand = process_shift_operand(operand_text);
    } else if(is_address(operand_text)) {
        ret_operand.type = ADDRESS;
        find_address_operand_vars(&ret_operand, operand_text);
    } else {  // must be label
        ret_operand.type = LABEL_NAME;
        ret_operand.value.label_name = operand_text;
    }
    return ret_operand;
}


instruction_data process_instruction(char *file_line) {
#define MAX_NO_OPS 4
    // instruction_data data = {.no_operands = 0};
    // int length = strlen(file_line);
    // char name[5];
    // int start_index = 0;
    // get_next_word(start_index, &start_index, file_line, name);  // replace with strtok, then check if there are commas at the end and remove them
    // data.instruction_name = name;
    // operand instr_operands[5];  // todo: check max operands
    // while(start_index < length) {
    //     char new_operand_text[64];  // not sure how big this should be
    //     get_next_word(start_index + 1, &start_index, file_line, new_operand_text);
    //     operand new_operand = process_operand(new_operand_text);
    //     instr_operands[data.no_operands] = new_operand;
    //     data.no_operands++;
    // }
    // data.operands = instr_operands;
    // return data;



    instruction_data data = {.no_operands = 0};
    char *current = strtok(file_line, " ");
    data.instruction_name = current;
    operand* operands_ptr = (operand*) malloc(MAX_NO_OPS * sizeof(operand));
    data.operands = operands_ptr;
    while((current = strtok(NULL, ",")) != NULL) {
        remove_space_from_operand(current);
        data.operands[data.no_operands] = process_operand(current);
        data.no_operands++;
        // operand temp;
        // if(*current == '#') {//operand is immediate value
        //     temp = immediateMake(atoi(current + 1));
        //     // assumes immediate value is valid, else is set to 0
        // } else if(is_special_register(current)) {
        //     temp.type = REGISTER;
        //     temp.value.register_operand.type = SPECIAL;
        //     if (strcmp(current + 1,"p") == 0) {
        //         temp.value.register_operand.id.special_register = SP;
        //         temp.value.register_operand.size = BIT_64;
        //     } else if (strcmp(current + 1,"sp") == 0) {
        //         temp.value.register_operand.id.special_register = SP;
        //         temp.value.register_operand.size = BIT_32;
        //     } else if (strcmp(current + 1,"zr") == 0) {
        //         temp.value.register_operand.id.special_register = ZR;
        //         temp.value.register_operand.size = (current[0] == 'x') ? BIT_64 : BIT_32;
        //     } else { // PC registers
        //         temp.value.register_operand.id.special_register = PC;
        //         temp.value.register_operand.size = BIT_64;
        //     }

        // } else if(is_general_register(current)) {
        //     temp = RegisterNsize(atoi(current + 1), *current);
        // } else if(is_shift_operation(current)) {
        //     temp.type = SHIFT;
        //     shift_info shiftInfo;
        //     shiftInfo.shift = get_shift_type(current);

        //     current = strtok(NULL, ds);
        //     if (*current == '#') {//operand is immediate value
        //         shiftInfo.shift_amount = atoi(current + 1);
        //         // assumes immediate value is valid, else is set to 0
        //     }
        //     temp.value.shift_operand = shiftInfo;
        // } else if(*current == '[') {//address this is th

        // } else {  // todo add label/directive variable things
        //     fprintf(stderr ,"operand %s is not real", current);
        // }
    }
    return data;
}

directive_data process_directive(char *file_line) {
    directive_data directive;
    directive.name = strtok(file_line, " ") + 1;
    char *arg_as_char = strtok(NULL, " ");
    directive.arg = (arg_as_char[1] == 'x') ? strtol(arg_as_char, NULL, 16) : atoi(arg_as_char);  // checks if hex and converts
    return directive;
}


// creates and returns the line_data structure so the line can be processed
line_data process_line(char *file_line) {
    int length = strlen(file_line);
    line_type type = get_line_type(file_line);
    line_data data = {.type = type};
    line_contents contents;
    switch(type) {
        case INSTRUCTION:
            contents.instruction = process_instruction(file_line);
            break;
        case DIRECTIVE:
            contents.directive = process_directive(file_line);  // remove first char (.)
            break;
        case LABEL:
            file_line[length - 1] = '\0';  // removes : by setting last char to NUL
            contents.label_name = file_line;
            break;
        default:
            printf("Something went very wrong (process_line)");
    }
    data.contents = contents;
    return data;
}

void process_input(char *input_file, line_data *line_tokens) {
    FILE *fp = fopen(input_file, "rb");
    if(fp != NULL) {
        char *line;
        int index = 0;
        while(fgets(line, 100, fp) != NULL) {
            line_data data = process_line(line);  // maybe this is a local var ????????????????????????
            line_tokens[index++] = data;
        }
    } else {
        fprintf(stderr, "Could not find the input file %s", input_file);
    }
    fclose(fp);
    printf("Succesfully assembled");
}


// int main( void ) {
//     printf("yay it compiles");
//     return 0;
// }