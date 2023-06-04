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


int in_string(char val, char *str) {
    for(int i = 0; i < strlen(str); i++) {
        if(val == str[i]) return 1;
    }
    return 0;
}

void get_next_word(char start, char* end, char* string, char** word) {
    int length = strlen(string);
    char end_characters[] = {' ', '\n', '\0'};
    for (int i = start; i < length ; i++) {
        if(~in_str(string[i], end_characters)) (*word)[i] = string[i];
        else {
            (*word)[i] = '\0';
            *end = i;  // technically the index of the end char after the word
            break;
        }
    }
}

void process_input(char *input_file, char *output_file) {
    FILE *fp = fopen(input_file, "rb");
    if(fp != NULL) {
        // something
    } else {
        printf("Could not find the input file %s. Please try again.", input_file);
    }
}

line_type get_line_type(char *file_line) {
    int length = strlen(file_line);
    if(file_line[0] == '.') return DIRECTIVE;
    if(file_line[length-2] == ':') return LABEL;
    return INSTRUCTION;
}

instruction_data process_instruction(char *file_line) {
    instruction_data data = {.no_operands = 0};
    int length = strlen(file_line);
    char name[5];
    int operand_index;
    get_next_word(0, &operand_index, file_line, &name);
    data.instruction_name = name;
    operand *instr_operands = {};
    while(operand_index < length) {
        char new_operand[64];  // not sure how big this should be
        get_next_word(operand_index + 1, &operand_index, file_line, &new_operand);
        instr_operands[data.no_operands] = new_operand;
        data.no_operands++;
    }
    data.operands = instr_operands;
    return data;
}


// creates and returns the line_data structure so the line can be processed
line_data process_line(char *file_line) {
    int length = strlen(file_line);
    line_type type = get_line_type(file_line);
    line_contents contents;
    switch (type) {
    case INSTRUCTION:
        contents.instruction_data = process_instruction(file_line);
        break;
    case DIRECTIVE:
        contents.directive_name = file_line + 1;  // remove first char (.)
    case LABEL:
        file_line[length - 1] = '\0';  // removes : by setting last char to NUL
        contents.label_name = file_line;
    default:
        printf("Something went very wrong (process_line)");
    }
    line_data data = {get_line_type(file_line)};
}

