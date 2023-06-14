#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "AssemblerInput.h"
#include "assemble.h"

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
operand shiftmake(shift_type type, int amount) {
    operand op;
    op.type = SHIFT;
    shift_info shiftop;
    shiftop.shift = type;
    shiftop.shift_amount = amount;
    op.value.shift_operand = shiftop;

    return op;
}

operand immediateMake(int n) {
    operand op;
    op.type = IMMEDIATE;
    op.value.immediate = n;
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

// void get_next_word(char start, int *end, char* string, char* word) {
//     #define END_CHARS_LENGTH 3
//     #define END_CHARS ", \n\0"
//     int length = strlen(string);
//
//     for (int i = start; i < length ; i++) {
//         if(~char_in_str(string[i], END_CHARS, END_CHARS_LENGTH)) word[i] = string[i];
//         else {
//             word[i] = '\0';
//             *end = i;  // technically the index of the end char after the word
//             break;
//         }
//     }
// }call regtostring(line.contents.instruction.operands[2])

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

shift_type get_shift_type(char* shift) {
    if (strcmp("lsl", shift) == 0) return LSL;
    else if (strcmp("lsr", shift) == 0) return LSR;
    else if (strcmp("asr", shift) == 0) return ASR;
    else if (strcmp("ror", shift) == 0) return ROR;
    else {
        perror("Unknown type of shift");
    }
}

operand process_operand(char* operand_text) {
    operand ret_operand;
    if(operand_text[0] == '#') {
        return immediateMake(atoi(operand_text + 1));
        // assumes immediate value is valid, else is set to 0
    } else if(is_special_register(operand_text)) {
        ret_operand.type = REGISTER;
        ret_operand.value.register_operand.type = SPECIAL;
        if (strcmp(operand_text + 1,"p") == 0) {
            ret_operand.value.register_operand.id.special_register = SP;
            ret_operand.value.register_operand.size = BIT_64;
        } else if (strcmp(operand_text + 1,"sp") == 0) {
            ret_operand.value.register_operand.id.special_register = SP;
            ret_operand.value.register_operand.size = BIT_32;
        } else if (strcmp(operand_text + 1,"zr") == 0) {
            ret_operand.value.register_operand.id.special_register = ZR;
            ret_operand.value.register_operand.size = (operand_text[0] == 'x') ? BIT_64 : BIT_32;
        } else { // PC registers
            ret_operand.value.register_operand.id.special_register = PC;
            ret_operand.value.register_operand.size = BIT_64;
        }
        return ret_operand;

    } else if(is_general_register(operand_text)) {
        return RegisterNsize(atoi(operand_text + 1), *operand_text);
    } else if(is_shift_operation(operand_text)) {
        ret_operand.type = SHIFT;

        // todo finish this
    } else {  // todo add label/directive variable things
        fprintf(stderr ,"operand %s is not real", operand_text);
    }


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
    const char ds[3]= ", ";
    char *current = strtok(file_line, ds);
    data.instruction_name = current;
    operand* operands_ptr = (operand*) malloc(MAX_NO_OPS * sizeof(operand));
    data.operands = operands_ptr;
    current = strtok(NULL, ds);
    while(current != NULL) {
        operand temp;

        if(*current == '#') {//operand is immediate value
            temp = immediateMake(atoi(current + 1));
            // assumes immediate value is valid, else is set to 0
        } else if(is_special_register(current)) {
            temp.type = REGISTER;
            temp.value.register_operand.type = SPECIAL;
            if (strcmp(current + 1,"p") == 0) {
                temp.value.register_operand.id.special_register = SP;
                temp.value.register_operand.size = BIT_64;
            } else if (strcmp(current + 1,"sp") == 0) {
                temp.value.register_operand.id.special_register = SP;
                temp.value.register_operand.size = BIT_32;
            } else if (strcmp(current + 1,"zr") == 0) {
                temp.value.register_operand.id.special_register = ZR;
                temp.value.register_operand.size = (current[0] == 'x') ? BIT_64 : BIT_32;
            } else { // PC registers
                temp.value.register_operand.id.special_register = PC;
                temp.value.register_operand.size = BIT_64;
            }

        } else if(is_general_register(current)) {
            temp = RegisterNsize(atoi(current + 1), *current);
        } else if(is_shift_operation(current)) {
            temp.type = SHIFT;
            shift_info shiftInfo;
            shiftInfo.shift = get_shift_type(current);

            current = strtok(NULL, ds);
            if (*current == '#') {//operand is immediate value
                shiftInfo.shift_amount = atoi(current + 1);
                // assumes immediate value is valid, else is set to 0
            }
            temp.value.shift_operand = shiftInfo;
        } else if(*current == '[') {//address this is th

        } else {  // todo add label/directive variable things
            fprintf(stderr ,"operand %s is not real", current);
        }
        data.operands[data.no_operands] = temp;


        data.no_operands++;
        current = strtok(NULL, ds);



    }
    return data;
}

directive_data process_directive(char *file_line) {
    directive_data directive;
    int arg_index; char *name;
//    get_next_word(0, &arg_index, file_line, name);
//    directive.name = name;
//    char *arg_as_char;
//    get_next_word(arg_index + 1, &arg_index, file_line, arg_as_char);
//    directive.arg = (arg_as_char[1] == 'x') ? strtol(arg_as_char, NULL, 16) : atoi(arg_as_char);  // checks if hex and converts
    return directive;
}


// creates and returns the line_data structure so the line can be processed
line_data process_line(char *file_line) {
    int length = strlen(file_line);
    line_type type = get_line_type(file_line);
    file_line = strdup(file_line);
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

 process_input(char *input_file, line_data *line_tokens) {
    FILE *fp = fopen(input_file, "rb");
    int index = 0;
    if(fp != NULL) {
        char line[100];
        while(fgets(line, 100, fp) != NULL && (strcmp(line, "\n") != 0)) {
            strtok(line, "\n");
            line_data data = process_line(line);  // maybe this is a local var ????????????????????????
            line_tokens[index++] = data;
        }
    } else {
        fprintf(stderr, "Could not find the input file %s", input_file);
    }
    fclose(fp);
    printf("Succesfully assembled");
    return index;
}


// int main( void ) {
//     printf("yay it compiles");
//     return 0;
// } run ../../armv8_testsuite/test/test_cases/general/add01.s output.bin
