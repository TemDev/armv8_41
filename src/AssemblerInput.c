// Handles the processing of the input to the assembler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "AssemblerInput.h"
#include "assemble.h"
#define MAX_NO_OPS 4




// converts string into int 
// can convert both decimal and decimal 
int32_t convert_immediate_value(char* imm_str) {
    return (imm_str[1] == 'x') ? strtol(imm_str, NULL, 16) : atoi(imm_str);
}
// construction of the immediate instructions
operand immediateMake(char* imm_str) {
     
    operand op;
    op.type = IMMEDIATE;
    op.value.immediate = convert_immediate_value(imm_str);
      // checks if hex and converts
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


//compares whether it has matched at least one string in the array
int32_t str_in_str_arr(char *str, char **str_arr, int32_t str_arr_len) {
    for(int32_t i = 0; i < str_arr_len; i++) {
        if(strlen(str) == strlen(str_arr[i])) {
            if (strcmp(str, str_arr[i]) == 0) return 1;  // if str is in str_arr
        }
    }
    return 0;
}

//Function that get rid of the unwanted spaces in the operand
static char* remove_space_from_operand(char *operand_text) {
    while(operand_text[0] == ' ') operand_text++;
    return operand_text;
}

//Function that figures out the type of the instruction to be performed in a certain line
line_type get_line_type(char *file_line) {
    file_line = remove_space_from_operand(file_line);
    
    while (file_line[strlen(file_line) - 1] == ' ') file_line[strlen(file_line) - 1] = '\0';
    if(file_line[0] == '.') return DIRECTIVE;
    if(file_line[strlen(file_line)-1] == ':') return LABEL;
    
    return INSTRUCTION;
}

//Figures out if an operand is a special register
int32_t is_special_register(char* operand_text) {
#define SRN_LEN 5
    char *SPECIAL_REGISTER_NAMES[] = {"sp", "wsp", "xzr", "wzr", "PC"};
    if(str_in_str_arr(operand_text, SPECIAL_REGISTER_NAMES, SRN_LEN)) return 1;
    return 0;
}

//Figures out if an operand is a general register
int32_t is_general_register(char* operand_text) {
    if(operand_text[0] == 'x' || operand_text[0] == 'w') {
        char *str;
        // attempts to convert it to the number
        long num = strtol(operand_text + 1, &str, 10);
        // if the string is not empty therefore it is not a register
        if (*str == '\0') {
            if (0 <= num && num < 31) return 1;
        }
        return 0;
    }
    return 0;
}

//Figures out if an operand is a shift operation
int32_t is_shift_operation(char* operand_text) {
#define SN_LEN 4
    char *SHIFT_NAMES[] = {"lsl", "lsr", "asr", "ror"};
    char shift_chars[4];
    strncpy(shift_chars, operand_text, 3);
    //
    if(str_in_str_arr(shift_chars, SHIFT_NAMES, SN_LEN) && operand_text[4] == '#') return 1;

    return 0;
}


//Figures out what type of shift is to be performed
shift_type get_shift_type(char* shift_text) {
    // determines by comparing fisrt 3 character what type of shift it is
    if (strncmp(shift_text, "lsl", 3) == 0) return LSL;
    else if (strncmp(shift_text, "lsr", 3) == 0) return  LSR;
    else if (strncmp(shift_text, "asr", 3) == 0) return ASR;
    else if (strncmp(shift_text, "ror", 3) == 0) return ROR;
    //any other value is invalid
    else fprintf(stderr, "ERROR in process shift unknown type of shift\n");
}

//Processing of a special register operand
operand process_special_register_operand(char* operand_text) {
    operand ret_op;
    ret_op.type = REGISTER;
    ret_op.value.register_operand.type = SPECIAL;
    // processes the special register names
    // +1 is added because the first character determines the size
    // eg. xzr, wzr
    if (strcmp(operand_text + 1,"p") == 0) {
        ret_op.value.register_operand.id.special_register = SP;
        ret_op.value.register_operand.size = BIT_64;
    } else if (strcmp(operand_text + 1,"sp") == 0) {
        ret_op.value.register_operand.id.special_register = SP;
        ret_op.value.register_operand.size = BIT_32;
    } else if (strcmp(operand_text + 1,"zr") == 0) {
        ret_op.value.register_operand.id.special_register = ZR;
        ret_op.value.register_operand.size = (operand_text[0] == 'x') ? BIT_64 : BIT_32;
    } else if (strcmp(operand_text + 1,"PC") == 0) {
        ret_op.value.register_operand.id.special_register = PC;
        ret_op.value.register_operand.size = BIT_64;
    }
    else fprintf(stderr, "ERROR in processing_special_register_operand unknown register\n");
    return ret_op;
}

//Processing of a shift
operand process_shift_operand(char *operand_text) {
    operand op;
    shift_info ret_shift;
    //determines and sets the shift type
    ret_shift.shift = get_shift_type(operand_text);
    //gets the numeric value, it starts at sixth character eg. "lsl #26"
    ret_shift.shift_amount = convert_immediate_value(operand_text+5);
    op.type = SHIFT;
    op.value.shift_operand = ret_shift;
    return op;
}

//Finding the addresses for the operand variables

//address can be one of the following forms
    //PRE  [xn, #<simm>]! operand_text:"[xn" next: "#<simm>]"!
    //POST [xn], #<simm> operand_text:"[xn]" next: "#<simm>]"
    //UNSIGNED [xn, #<imm>] or [xn] operand_text:"[xn, #<imm>]" next: ""
    //REG_SHIFT [xn, xm] operand_text:"[xn, xm]" next: ""
    //LOAD <literal> literal is either a value or label,
    //so it would get processed as a regular operand
void find_address_operand_vars(operand *addr_operand, char* operand_text, char* next) {
    offset_type type;
    bool sndReg = false;
    // gets and sets the first register of the address 
    char *reg = strtok(operand_text, ",");
    if (reg[strlen(reg) -1 ] ==   ']') {
        reg[strlen(reg) -1 ] = '\0';
    }
    addr_operand -> value.address1.operand1 = RegisterNsize(atoi(reg + 2), *(reg + 1)).value.register_operand;
    // reg now is the second token in operand_text
    reg = strtok(NULL, " ");
    char* op2;
    char *ref;
    //selects where to choose the second value from
    //either from next or second token of the operand_text
    char* reference = (reg != NULL)? reg : next;
    if (reference != NULL) {
        ref = strdup(reference);
        // gets the next value which also trims off the delimeters specified
        // op2 is the string that is going to be converted into 
        // the second operand of address
        op2 = strtok(ref, ", ]");
    } else {
        // there is no second value, therefore unsigned offset
        op2 = NULL;
    }
    if (op2 != NULL){
    // analyses and converts second token into correct type register or immediate
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
        // othernwise it is zero unsigned offset which is the same as offset by 0
        addr_operand -> value.address1.operand2.immediate_value = 0;
    }
    // determines the type of the offset
    if (!reference) {
        type = UNSIGNED;
    } else if (reference[strlen(reference) - 1] == '!') {
        type = PRE;
    } else if (reference[strlen(reference) - 1] == ']'){
        if (sndReg) {
            type = REG_SHIFT;
        } else {
            type = UNSIGNED;
        }

    } else {
        type = POST;
    }
    
    addr_operand -> type = ADDRESS;

    addr_operand -> value.address1.address_type = type;
    
}

//Processing of any operand
operand process_operand(char* operand_text) {
    operand ret_operand;
    if(*operand_text == '#') {
        // processes the string starting after '#' character
        ret_operand = immediateMake(operand_text + 1);
    } else if(is_special_register(operand_text)) {
        ret_operand = process_special_register_operand(operand_text);
    } else if(is_general_register(operand_text)) {
        //general register processes the numeric value starting at second character
        //and also takes into a account the first character which determines its size
        ret_operand = RegisterNsize(atoi(operand_text + 1), *operand_text);
    } else if(is_shift_operation(operand_text)) {
        ret_operand = process_shift_operand(operand_text);
    } else {  
        // otherwise it has to be the label
        ret_operand.type = LABEL_NAME;
        ret_operand.value.label_name = operand_text;
    }
    return ret_operand;
}

//Processing of any type of instruction
instruction_data process_instruction(char *file_line) {

    instruction_data data = {.no_operands = 0};
    //gets and sets the first token as the name of the instruction
    char *current = strtok(file_line, " ");
    data.instruction_name = current;
    //allocates a space for the operands on the heap
    operand* operands_ptr = (operand*) malloc(MAX_NO_OPS * sizeof(operand));
    data.operands = operands_ptr;
    while((current = strtok(NULL, ",")) != NULL) {
        //trims of the spaces from the line
        current = remove_space_from_operand(current);
        //if token starts with '[' then it is an address and it gets process as address
        if (*current == '[') {
            //address requires two next tokens to get processed 
            char *oldcurr = malloc(strlen(current) + 1);
            //copies the current into the oldcurr
            strcpy(oldcurr, current);
            // gets the value of the new token
            current = strtok(NULL, ", ");   
            //processes the address operand with two tokens     
            find_address_operand_vars(&data.operands[data.no_operands],oldcurr, current);
            //address is always the last operand, so this ensures that it terminates
            current = strtok(NULL, ", ");
        } else {
            //processes and sets new operand
            data.operands[data.no_operands] = process_operand(current);
        }
        data.no_operands++;
    }
    return data;
}

//Processing of a directive
directive_data process_directive(char *file_line) {
    directive_data directive;
    // takes the first token which is .int
    char * temp =strtok(file_line, " ");
    // checks that ther first token is .int and nothing else
    // NOTE: HAS TO BE CHANGED IF EXTENDING THE LANGUAGE SET
    assert(strcmp(temp, ".int") == 0);
    // gets the next token which is the number stored
    temp = strtok(NULL, " ");
    // if 2nd character is a x then processes it as hex, otherwise as denary
    directive.arg = (temp[1] == 'x') ? strtol(temp, NULL, 16) : atoi(temp);
    return directive;
}


// creates and returns the line_data structure so the line can be processed
line_data process_line(char *file_line) {
    //creates a duplicate of the line on the heap
    file_line = strdup(file_line);
    
// determines which line type it is and sets it
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
        // takes the label without ':' at the end
            file_line = strtok(file_line, " :"); 
            contents.label_name = file_line;
            break;
        default:
            fprintf(stderr, "ERROR in process_line Invalid type of the line data\n");
    }
    data.contents = contents;
    return data;
}

// Function that processes the input file and returns the number of non-empty lines
int32_t process_input(char *input_file, line_data *line_tokens) {
    FILE *fp = malloc(sizeof(FILE));
    fp = fopen(input_file, "r");
    int32_t index = 0;
    if(fp != NULL) {
        char line[100];
        while(fgets(line, 100, fp) != NULL) {
            // makes sure that spaces are not included into command processing
            char* isnewline = remove_space_from_operand(line);
            // newlines should not be processed
            if (strcmp(isnewline,"\n") == 0) {
                continue;
            }
            //trims of the new line
            strtok(line, "\n");
            line_tokens[index++] = process_line(line);
        }
    } else {
        fprintf(stderr, "Could not find the input file %s", input_file);
    }
    fclose(fp);
    return index;
}
