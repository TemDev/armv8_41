#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bitwise.c"
#include "AssemblerDecoder.h"
#include "assemble.h"
// return a string that is a binary representation of a number

void toBinaryPrint(int number) {
	int temp = 1;
	char string[33];
	// a problem if less than 32 character string is making that
	//assert(strlen(string) >= 32);
	for (int i = 0; i < 32; i++) {
		if (((number >> i) & 1 ) == 1) {
	       		string[31 - i] = '1';
	       	}
			else string[31 -i] = '0';
		temp = temp << 1;
	}
	string[32] = '\0';
	printf("%.8x in binary is %32s \n", number, string);
	
}


typedef int (*instructionMaker) (instruction_data*,char);
typedef struct {
    char* key;
    instructionMaker function;
    char opcode;
} instToFunction;

char getRegisterNumber(char index, instruction_data* inst) {
	assert(inst -> no_operands > index);
	register_info reg = inst -> operands[index].value.register_operand;
	if (reg.type == GENERAL) {
		return reg.id.number;
	} else if (reg.type == SPECIAL) {
		return 31;
	} else {
		printf("Register expected other type provided\n");
		exit(1);
	}
	
}

// opcodes refers to a type of a branch
// opcode values 3 and 4 are not used by conditional branches
// so they are used to identify register and uncoditional branches
int BR(instruction_data *inst, char opcode) {
	int temp = 0;
	printf("BR is called\n");
	assert(inst-> no_operands > 0);
	if (opcode == 3) {
		int simm26 = inst -> operands[0].value.immediate;
	       temp = (5 << 26) + ((simm26 & (1 << 26)-1));	
	} else if (opcode == 4) {
		// don't forget to check which register specific 
		char xn = inst -> operands[0].value.register_operand.id.number & 31;
		 temp = (107 << 25) + (31 << 16) +  (xn << 5);
 		 printf("Branch Reister %x\n",xn);

	}
	else {
		int simm19 = inst -> operands[0].value.immediate;
		printf("%x \n", simm19 & (( 1 << 20 ) - 1)); 
		temp = (21 << 26) + (((( 1 << 20 ) - 1) & simm19) << 5) + (31 & opcode);
	}	
	return temp;
}

int Transfer(instruction_data *inst, char opcode){
	offset_type type = inst -> operands[1].value.address1.address_type;
	bool load  = (inst -> operands[1].type != ADDRESS);
	int temp = 0;
	char sf = (inst -> operands[0].value.register_operand.size == BIT_64)? 1:0;
	char U = (type == UNSIGNED)? 1:0;
	char I = (type == PRE)? 1:0;
	char L = opcode & 1;
	
	if (load) { 
		temp = ((1 & sf) << 3) + 3;
		int simm19 = ((inst -> operands[1].value.immediate) & ((1 << 19)-1));
		temp = (temp <<27) + (simm19 << 5);
	} else {
		temp = ((1 & sf) << 3) + 23;
		int offset = 0;
		if (type == UNSIGNED) {
			 offset = inst -> operands[1].value.address1.operand2.immediate_value & ((1 << 12) - 1);
		} else if (type == REG) {
			// proccess special registers
			char xm = (inst -> operands[1].value.address1.operand1.type == SPECIAL)? 
			31 :inst -> operands[1].value.address1.operand2.register_value.id.number;
			offset = (1 << 11) + (xm << 6) + 26;
		} else {
			int simm9 = inst -> operands[1].value.address1.operand2.immediate_value & ((1 << 9) - 1);
			offset = (simm9 << 2 )+ ((I & 1) << 1) + 1;
		}
// process special registers
		char xn = (inst -> operands[1].value.address1.operand1.type == SPECIAL)? 
			31 : inst -> operands[1].value.address1.operand1.id.number;
		temp = (temp << 27) +  ((1 & U) << 24)  + ((1 & L) << 22) + (offset << 10) + (xn << 5);
	
	}
	char rt = getRegisterNumber(0, inst);
	temp += rt;
	
    return temp;
}
int DP(instruction_data *inst, char opcode){
	//is it register or immediate
    bool isReg = (REGISTER == (inst -> operands[2].type)) &&
            (REGISTER == (inst -> operands[1].type));
	// add assertions that it has enough operand
	bool is64 = BIT_64 == (inst -> operands[0].value.register_operand.size);
	int temp = 0;
	char opi, opc, opr, I, R, N, shift, operand;
	opc = 3 & opcode;// get last 2 bits;
	I =( opcode >> 2) & 1;
	R = (opcode >> 3) & 3;
	N = (opcode >> 5) & 1;
	
	// checks whether it is Bit Logic, Arithmetic or Multiply;
	temp = (is64)? 4: 0;
    temp += opc;
	//printf("R:%d opcode: %d \n", R, opcode);

	// adding an instruction opcode
	if (isReg) {
        printf("Register\n");
        temp = (temp << 4) + 5;
		char rm = getRegisterNumber(2, inst); 
		char rn = getRegisterNumber(1, inst); 
		char rd = getRegisterNumber(0, inst);
		

        if (R == 1) {
            char ra = getRegisterNumber(3, inst);
            // adding an opcode for DP and an opc for multiply
            temp = (temp << 4) + 8 + 128;
            //adding rm and x bit named N here and ra
            temp = (temp << 21) + (rm << 16) + ((N & 1) << 15) +( ra << 10) + (rn << 5) + rd;
            printf("Performing a Multiply instruction\n");
        } else if (R == 0 || R == 2){
            if (inst -> no_operands > 3) {
                shift = (char) inst -> operands[3].value.shift_operand.shift;
                operand =  inst -> operands[3].value.shift_operand.shift_amount & 63;
            } else {
                shift = 0;
                operand = 0;
            }

            temp = (temp << 4) + ((shift & 3) << 1) ;
            if (R == 0) {
                temp += N & 1;
                printf("Performing a logical instruction\n");
            } else {
                temp += 8;
                printf("Performing a arithmetic instruction in registers\n");
            }
            //adding rm and operand
            temp = (temp << 21) + (rm  << 16) + (operand << 10) + (rn << 5) + rd;

        } else {
            exit(1);
        }
	} else {

		temp = (temp << 3) + 4;
		// adds opcode of DP immediate value
        char rd = getRegisterNumber(0, inst);


		if (I == 0) { // arithmetic operation
            // adds opi
			temp = (temp << 3) + 2;

            char rn = getRegisterNumber(1, inst);
            int imm12 = ((inst -> operands[2].value.immediate) & ((1 << 12)-1));
            char sh = (inst -> no_operands > 3)? 1 : 0;


			// the flag whether it should be shifted
			//adding sh and imm12
            temp = (temp << 23) + ((sh & 1 ) << 22) + (imm12 << 10) + (rn << 5) + rd;
			       	
			// adding rn and rd
            // register number is not correct
			printf("Performing an arithmetic instruction\n");
		} else if (I == 1) { // wide move
            // adds opi to the  instructions
			temp = (temp << 3) + 5;
			// value by which it should be shifted
            char hw;

            if (inst -> no_operands > 2) {
                hw = ((inst -> operands[2].value.shift_operand.shift_amount & 63) >> 4);
            } else {
                hw = 0;
            }
			// immediate value 
			int imm16 = ((inst -> operands[1].value.immediate) & ((1 << 16)-1));
		       	
			temp = (temp  << 23) + (hw << 21) + (imm16 << 5) + rd;
		       	// i need to get register number;			
			printf("Performing a wide move\n");
		}
	}
	toBinaryPrint(temp);
	return temp;
}
instToFunction instToFunctions[30] = {
	{"add", &DP, 16}, {"adds", &DP, 17}, {"sub", &DP, 18}, {"subs", &DP, 19}, 
       	{"and", &DP, 0}, {"ands", &DP, 3}, {"bic", &DP, 32}, {"bics", &DP, 35},
	{"eor", &DP, 2}, {"eon", &DP, 34}, {"orr", &DP, 1}, {"orn", &DP, 33},
	{"movn", &DP, 4}, {"movk", &DP, 7}, {"movz", &DP, 6}, {"madd", &DP, 8},
	{"msub", &DP, 40}, 
	{"b", &BR, 3}, {"br", &BR, 4}, {"b.eq", &BR, 0}, {"b.ne", &BR, 1},
	{"b.ge", &BR, 10}, {"b.lt", &BR, 11}, {"b.gt", &BR, 12}, {"b.le", &BR, 13}, 
	{"b.al", &BR, 14}, 
	{"ldr", &Transfer, 1}, {"str", &Transfer, 0},{"nop", &Transfer, 7}}; 
	

int decode(instruction_data inst) {
	
	for (int i = 0; i < 30; i++) {
		if (strcmp(instToFunctions[i].key,inst.instruction_name) == 0) {
			instructionMaker fun = (instToFunctions[i].function);

			return fun(&inst, instToFunctions[i].opcode);
		}
	}
	// it means unknown instruction passed
	// it should never reach this value	
	assert(false);
	return -1;
}

int decodeline(line_data line) {
    switch (line.type) {
        case DIRECTIVE:
            return 0;
            break;
        case INSTRUCTION:
            return decode(line.contents.instruction);
            break;
        default:
            return 0;
    }
}
// run ../../armv8_testsuite/test/test_cases/general/add01.s output.bin