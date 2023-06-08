#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { INSTRUCTION, DIRECTIVE, LABEL } line_type;

typedef enum { REGISTER, IMMEDIATE } operand_type;

typedef enum { GENERAL, SPECIAL } register_type;

typedef enum { SP, ZR, PC } special_register_type;

typedef enum { BIT_64, BIT_32 } register_size;

typedef union { special_register_type special_register; int number; } register_id;

typedef struct { register_type type; register_id id; register_size size; } register_info;

typedef union { register_info register_operand; int64_t immediate; } operand_value;

typedef struct { operand_type type; operand_value value; } operand; 

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

void get_next_word(char start, int *end, char* string, char* word) {
    #define END_CHARS_LENGTH 3
    #define END_CHARS " \n\0"
    int length = strlen(string);
    
    for (int i = start; i < length ; i++) {
        if(~char_in_str(string[i], END_CHARS, END_CHARS_LENGTH)) word[i] = string[i];
        else {
            word[i] = '\0';
            *end = i;  // technically the index of the end char after the word
            break;
        }
    }
}

line_type get_line_type(char *file_line) {
    int length = strlen(file_line);
    if(file_line[0] == '.') return DIRECTIVE;
    if(file_line[length-2] == ':') return LABEL;
    return INSTRUCTION;
}

int is_register(char* operand_text) {
    #define SRN_LEN 5
    char *SPECIAL_REGISTER_NAMES[] = {"sp", "wsp", "xzr", "wzr", "PC"};
    if(str_in_str_arr(operand_text, SPECIAL_REGISTER_NAMES, SRN_LEN)) return 1;
    if(operand_text[0] == 'x' || operand_text[0] == 'w') {
        char *str;
        long num = strtol(operand_text + 1, &str, 10); 
        if (*str != num && *str == '\0') {
            if (0 <= num && num <= 31) return 1;
        }
    }
    return 0;
}

operand process_operand(char* operand_text) {
    operand ret_operand;
    if(operand_text[0] == '#') {
        ret_operand.type = IMMEDIATE;
        ret_operand.value.immediate = atoi(operand_text + 1);  // assumes immediate value is valid, else is set to 0
    } else if(is_register(operand_text)) {
        ret_operand.type = REGISTER;
        if(operand_text == "sp") {
            ret_operand.value.register_operand.type = SPECIAL;  // move this
        } 
    }


}

instruction_data process_instruction(char *file_line) {
    instruction_data data = {.no_operands = 0};
    int length = strlen(file_line);
    char name[5];
    int operand_index;
    get_next_word(0, &operand_index, file_line, name);
    data.instruction_name = name;
    operand instr_operands[5];  // todo: check max operands
    while(operand_index < length) {
        char new_operand_text[64];  // not sure how big this should be
        get_next_word(operand_index + 1, &operand_index, file_line, new_operand_text);
        operand new_operand = process_operand(new_operand_text);
        instr_operands[data.no_operands] = new_operand;
        data.no_operands++;
    }
    data.operands = instr_operands;
    return data;
}

directive_data process_directive(char *file_line) {
    directive_data directive;
    int arg_index; char *name;
    get_next_word(0, &arg_index, file_line, name); directive.name = name;
    char *arg_as_char;
    get_next_word(arg_index + 1, &arg_index, file_line, arg_as_char);
    directive.arg = atoi(arg_as_char);   // todo check assembly to make sure this is right
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
    case LABEL:
        file_line[length - 1] = '\0';  // removes : by setting last char to NUL
        contents.label_name = file_line;
    default:
        printf("Something went very wrong (process_line)");
    }
    data.contents = contents;
    return data;
}

void process_input(char *input_file, char *output_file) {
    FILE *fp = fopen(input_file, "rb");
    if(fp != NULL) {
        char *line;
        while(fgets(line, 100, fp) != NULL) {
            line_data data = process_line(line);
            // call function with (data) here
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