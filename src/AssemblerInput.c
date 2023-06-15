#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "AssemblerInput.h"
#include "assemble.h"
#define MAX_NO_OPS 4

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
/*operand RegisterZR(char c) {
    operand op;
    op.type = REGISTER;
    register_info reg;
    reg.type = SPECIAL;
    reg.size = BIT_64;
    register_id id;
    id.special_register = ZR;
    op.value.register_operand = reg;
    return op;
}*/
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
operand shiftmake(shift_type type, int32_t amount) {
    operand op;
    op.type = SHIFT;
    shift_info shiftop;
    shiftop.shift = type;
    shiftop.shift_amount = amount;
    op.value.shift_operand = shiftop;

    return op;
}

operand immediateMake(char* imm_str) {
     
    operand op;
    op.type = IMMEDIATE;
    op.value.immediate = (imm_str[1] == 'x') ? strtol(imm_str, NULL, 16) : atoi(imm_str);
      // checks if hex and converts
    return op;
}
// built in func exists strchr BUT don't think it works as can't check if '\0' ?
int32_t char_in_str(char val, char *str, int32_t str_len) {
    for(int i = 0; i < str_len; i++) {
        if(val == str[i]) return 1;  // if char is in str
    }
    return 0;
}


int32_t str_in_str_arr(char *str, char **str_arr, int32_t str_arr_len) {
    for(int32_t i = 0; i < str_arr_len; i++) {
        if(strlen(str) == strlen(str_arr[i])) {
            if (strcmp(str, str_arr[i]) == 0) return 1;  // if str is in str_arr
        }
    }
    return 0;
}

static char* remove_space_from_operand(char *operand_text) {
    while(operand_text[0] == ' ') operand_text++;
    return operand_text;
}


line_type get_line_type(char *file_line) {
    file_line = remove_space_from_operand(file_line);
    
    while (file_line[strlen(file_line) - 1] == ' ') file_line[strlen(file_line) - 1] = '\0';
    if(file_line[0] == '.') return DIRECTIVE;
    if(file_line[strlen(file_line)-1] == ':') return LABEL;
    
    return INSTRUCTION;
}

int32_t is_special_register(char* operand_text) {
#define SRN_LEN 5
    char *SPECIAL_REGISTER_NAMES[] = {"sp", "wsp", "xzr", "wzr", "PC"};
    if(str_in_str_arr(operand_text, SPECIAL_REGISTER_NAMES, SRN_LEN)) return 1;
    return 0;
}

int32_t is_general_register(char* operand_text) {
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

int32_t is_shift_operation(char* operand_text) {
#define SN_LEN 4
    char *SHIFT_NAMES[] = {"lsl", "lsr", "asr", "ror"};
    char shift_chars[4];
    strncpy(shift_chars, operand_text, 3);
    if(str_in_str_arr(shift_chars, SHIFT_NAMES, SN_LEN) && operand_text[4] == '#') return 1;
    return 0;
}

int32_t is_address(char *operand_text) {
    return (operand_text[0] == '[') ? 1 : 0;
}

shift_type get_shift_type(char* shift) {
    if (strcmp("lsl", shift) == 0) return LSL;
    else if (strcmp("lsr", shift) == 0) return LSR;
    else if (strcmp("asr", shift) == 0) return ASR;
    else if (strcmp("ror", shift) == 0) return ROR;
    else {
        perror("Unknown type of shift");
        return ROR;
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
    
    ret_shift.shift_amount = immediateMake(operand_text+5).value.immediate;
    return ret_shift;
}

void find_address_operand_vars(operand *addr_operand, char* operand_text, char* next) {
    offset_type type;
    bool sndReg = false;
    char *reg = strtok(operand_text, ",");
    if (reg[strlen(reg) -1 ] ==   ']') {
        reg[strlen(reg) -1 ] = '\0';
    }
    addr_operand -> value.address1.operand1 = RegisterNsize(atoi(reg + 2), *(reg + 1)).value.register_operand;
    reg = strtok(NULL, " ");
    char* op2;
    char *ref;
    char* reference = (reg != NULL)? reg : next;
    if (reference != NULL) {
        ref = strdup(reference);
        op2 = strtok(ref, ", ]");
    } else {
        op2 = NULL;
    }
    if (op2 != NULL){
    if (is_general_register(op2)) {
        sndReg = true;
        addr_operand -> value.address1.operand2.register_value 
        = RegisterNsize(atoi(op2 + 1), *(op2)).value.register_operand;
        
    } else if (is_special_register(op2)) {
        sndReg = true;
        addr_operand -> value.address1.operand2.register_value = process_special_register_operand(op2).value.register_operand;
    } else if (*op2 == '#') {
        
        addr_operand -> value.address1.operand2.immediate_value = immediateMake(op2 + 1).value.immediate;
    }
    } else {
        addr_operand -> value.address1.operand2.immediate_value = 0;
    }
    if (!reference) {
        type = UNSIGNED;
    } else if (reference[strlen(reference) - 1] == '!') {
        type = PRE;
    } else if (reference[strlen(reference) - 1] == ']'){
        if (sndReg) {
            type = REG;
        } else {
            type = UNSIGNED;
        }

    } else {
        type = POST;
    }
    addr_operand -> type = ADDRESS;

    addr_operand -> value.address1.address_type = type;
    
}


operand process_operand(char* operand_text) {
    operand ret_operand;
    if(operand_text[0] == '#') {
        return immediateMake(operand_text + 1);
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
        find_address_operand_vars(&ret_operand, operand_text, "");
    } else {  // must be label
        ret_operand.type = LABEL_NAME;
        ret_operand.value.label_name = operand_text;
    }
    return ret_operand;
}


instruction_data process_instruction(char *file_line) {
    // instruction_data data = {.no_operands = 0};
    // int32_t length = strlen(file_line);
    // char name[5];
    // int32_t start_index = 0;
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
    while(((current = strtok(NULL, ",")) != NULL) && (strcmp(current, "!") != 0)) {
        current = remove_space_from_operand(current);
        if (*current == '[') {
            char *oldcurr = malloc(strlen(current) + 1);
            strcpy(oldcurr, current);
            current = strtok(NULL, ", ");
            find_address_operand_vars(&data.operands[data.no_operands],oldcurr, current);
            current = strtok(NULL, ", ");
        } else {
            operand temp = process_operand(current);
            data.operands[data.no_operands] = temp;

        }
        data.no_operands++;
    }
    return data;
}

directive_data process_directive(char *file_line) {
    directive_data directive;
    char * temp =strtok(file_line, " ");
    directive.name = strdup(temp);
    temp = strtok(NULL, " ");
    directive.arg = (temp[1] == 'x') ? strtol(temp, NULL, 16) : atoi(temp);  // checks if hex and converts
    return directive;
}


// creates and returns the line_data structure so the line can be processed
line_data process_line(char *file_line) {
    file_line = strdup(file_line);
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
            file_line = strtok(file_line, " :");  // removes : by setting last char to NUL
            contents.label_name = file_line;
            break;
        default:
            printf("Something went very wrong (process_line)");
    }
    data.contents = contents;
    return data;
}

int32_t process_input(char *input_file, line_data *line_tokens) {
    FILE *fp = fopen(input_file, "r");
    int32_t index = 0;
    if(fp != NULL) {
        char line[100];
        while(fgets(line, 100, fp) != NULL) {
            char* isnewline = remove_space_from_operand(line);
            if (strcmp(isnewline,"\n") == 0) {
                continue;
            }
            strtok(line, "\n");
            line_data data = process_line(line);  // maybe this is a local var
            line_tokens[index++] = data;
        }
    } else {
        fprintf(stderr, "Could not find the input file %s", input_file);
    }
    fclose(fp);
    printf("Succesfully assembled");
    return index;
}
