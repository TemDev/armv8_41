#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// built in func exists strchr BUT don't think it works as can't check if '\0' ?
int char_in_str(char val, char *str, int str_len) {
    for(int i = 0; i < str_len; i++) {
        if(val == str[i]) return 1;  // if char is in str
    }
    return 0;
}

// built in func exists strcmp
// int compare_str(char *str1, char *str2) {
//     for(int j = 0; j < strlen(str1); j++) {
//         if(str1[j] != str2[j]) {
//             return 0;
//         }
//     return 1;  // if two strings are equal
//     }
// }

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
    
//     for (int i = start; i < length ; i++) {
//         if(~char_in_str(string[i], END_CHARS, END_CHARS_LENGTH)) word[i] = string[i];
//         else {
//             word[i] = '\0';
//             *end = i;  // technically the index of the end char after the word
//             break;
//         }
//     }
// }

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
        if (*str != num && *str == '\0') {
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

operand process_operand(char* operand_text) {
    operand ret_operand;
    if(operand_text[0] == '#') {
        ret_operand.type = IMMEDIATE;
        ret_operand.value.immediate = atoi(operand_text + 1);  // assumes immediate value is valid, else is set to 0
    } else if(is_special_register(operand_text)) {
        ret_operand.type = REGISTER;
        ret_operand.value.register_operand.type = SPECIAL;
        switch(operand_text + 1) {  // remove letter to catch same type registers
        case "p":
            ret_operand.value.register_operand.id.special_register_type = SP;
            ret_operand.value.register_operand.size = BIT_64;
            break;
        case "sp":
            ret_operand.value.register_operand.id.special_register_type = SP;
            ret_operand.value.register_operand.size = BIT_32;
            break;
        case "zr":
            ret_operand.value.register_operand.id.special_register_type = ZR;
            ret_operand.value.register_operand.size = (operand_text[0] == 'x') ? BIT_64 : BIT_32;
            break;
        default:  // must be PC
            ret_operand.value.register_operand.id.special_register_type = PC;
            ret_operand.value.register_operand.size = BIT_64;
            break;
        }
    } else if(is_general_register(operand_text)) {
        ret_operand.type = REGISTER;
        ret_operand.value.register_operand.type = GENERAL;
        ret_operand.value.register_operand.size = (operand_text[0] == 'x') ? BIT_64 : BIT_32;
        ret_operand.value.register_operand.register_id.number = atoi(operand+1);
    } else if(is_shift_operation(operand_text)) { 
        ret_operand.type = SHIFT;  // set shift_operand.shift, shinft_operand.amount type and amount
        char shift_chars[4];
        strncpy(shift_chars, operand_text, 3);
        case (shift_chars) {
            switch "lsl": ret_operand.value.shift_operand.shift = LSL; break;
            switch "lsr": ret_operand.value.shift_operand.shift = LSR; break;
            switch "asr": ret_operand.value.shift_operand.shift = ASR; break;
            switch "ror": ret_operand.value.shift_operand.shift = ROR; break;
        }
        ret_operand.val.shift_operand.amount = atoi(operand_text+5);
        
    } else {  // todo add label/directive variable things
        fprintf(stderr ,"operand %s is not real", operand_text);
    }


}

instruction_data process_instruction(char *file_line) {
    #define MAX_NO_OPS 4;
    // instruction_data data = {.no_operands = 0};
    // int length = strlen(file_line);
    // char name[5];
    // int start_index = 0;
    // get_next_word(start_index, &start_index, file_line, name);  // replace with strtok, then check if there are commas at the end and remove them
    // data.instruction_name = name;
    // operand instr_operands[5]; 
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
    int start_index = 0;
    char *instr_words = strtok(file_line, " ");
    data.instruction_name = instr_words[0];
    operand* operands_ptr = (operand*) malloc(MAX_NO_OPS * sizeof(operand));
    while(instr_words != NULL) {
        instr_words = strtok(NULL, " ");
        operands_ptr[data.no_operands] = new_operand;
        data.no_operands++;
    }
    data.operands = operands_ptr;
    return data;
}

directive_data process_directive(char *file_line) {
    directive_data directive;
    int arg_index; char *name;
    get_next_word(0, &arg_index, file_line, name); directive.name = name;
    char *arg_as_char;
    get_next_word(arg_index + 1, &arg_index, file_line, arg_as_char);
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

void process_input(char *input_file, char *output_file, line_data *line_tokens) {
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