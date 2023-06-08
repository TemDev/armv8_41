#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "bitwise.c"
#include "types.h"
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
	printf("%d or %x in binary is %s \n", number, number, string);
	
}
typedef int (*instructionMaker) (instruction_data*,char);
typedef struct {
	char* key;
	instructionMaker *function;
	char opcode; 
// opcode is of the form for 
// DP : |N|R|I|opc|
} instToFunction;

typedef struct {
	char opc;
	char opi;
	
	int operand;
	char rd;
		// how DP_immediate insturction looks
		// sf|opc|100|opi|operand|rd
		// 1 | 3 | 3 | 3 | 18    |5
} DP_Immediate;

typedef struct {
	char opc;
	char opr;
	char rm;
	char operand;
	char rn;
	char rd;
		// how DP_Register insturction looks
		// sf|opc|1001|opr|rm|operand|rn|rd
		// 1 | 3 | 4  | 4 | 5| 6     | 5| 5
} DP_Register;

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
	return 0;
}
int DP(instruction_data *inst, char opcode){
	//is it register or immediate
	assert(inst -> no_operands > 3); // not enough operands
	bool isReg = REGISTER == (inst -> operands[2].type);
	// add assertions that it has enough operand
	bool is64 = BIT_64 == (inst -> operands[0].value.register_operand.size);
	int temp = 0;
	char opi, opc, opr, I, R, N, shift, operand;
	opc = 3 & opcode;// get last 2 bits;
	I =( opcode >> 2) & 1;
	R = (opcode >> 3) & 3;
	N = (opcode >> 5) & 1;
	opi = (I == 1)? 5: 2;
	opr = (R == 0)? 0: 8; // checks whether it is Bit Logic, Arithmetic or Multiply;
	temp = (is64)? 4: 0;
       	temp += opc;
	printf("R:%d opcode: %d \n", R, opcode);

	temp = (temp << 4) + 5;// adding an instruction opcode	
	if (isReg) {
		char rm = ((inst -> operands[2].value.register_operand.id.number) & 31); 
		char rn = ((inst -> operands[1].value.register_operand.id.number) & 31); 
		char rd = ((inst -> operands[0].value.register_operand.id.number) & 31);
			
		switch (R) {
			case 0 :
			shift = 0 ; //inst -> operands[4].typeShift
			operand = ((inst -> operands[3].value.immediate) & 63);
			temp = ((temp << 4) + ((shift & 3) < 1) + N & 1);
			//adding rm and operand
		 	temp = temp << 21 + rm  << 16 + operand << 10 + rn << 5 +rd;       
			printf("Performing a logical instruction\n");
			
			break;
			case 2 :// arithmetic
			shift = 0; 
			operand = ((inst -> operands[3].value.immediate) & 63);
			
			temp = (temp << 4) + ((shift & 3) < 1);
			//adding rm and operand
		       	temp = (temp << 21) + rm  << 16	+ operand << 10 + rn << 5 +rd; 
			printf("Performing an arithmetic instruction\n");
			break;
			case 1: //multiply
			char ra = (inst -> operands[3].value.register_operand.id.number & 31);
			// adding an opcode for DP and an opc for multiply 
			temp = ((temp << 5) + 11) << 3;
			//adding rm and x bit named N here and ra
		       	temp = (temp << 21) + rm << 16 + ((N & 1) << 15) + ra << 10 + rn << 5 + rd;
			printf("Performing a logical instruction\n");
			break;
			default:
			exit(1);
		}

		 

	} else {
		temp = (temp << 1 + 1) << 2;
		// adds opcode of DP immediate value
		char rd = (inst -> operands[0].value.register_operand.id.number & 31);

		if (I == 0) { // arithmetic operation
			      // adds opi
			temp = temp << 3 + 2;
			char sh = 0;
		       	int imm12 = ((inst -> operands[2].value.immediate) & ((1 << 12)-1));
			// the flag whether it should be shifted
			//adding sh and imm12
		       	temp = (temp << 13) + ((sh  & 1 ) << 12) + imm12;
			       	
			// adding rn and rd
			char rn =  ((inst -> operands[1].value.immediate) & 31); 
					// register number is not correct
			temp = (temp << 10) + rn << 5 + rd; 
			printf("Performing an arithmetic instruction\n");
		} else if (I == 1) {

			temp = temp << 3 + 5;
			// value by which it should be shifted
			char hw = 0;
			// immediate value 
			int imm16 = ((inst -> operands[2].value.immediate) & ((1 << 16)-1));
		       	
			temp = (temp  << 23) + (hw << 21) + imm16 << 5 + rd;
		       	// i need to get register number;			
			printf("Performing a wide move\n");
		}
	}
	toBinaryPrint(temp);
	return 0;
}
instToFunction instToFunctions[30] = {
	{"add", &DP, 16}, {"adds", &DP, 17}, {"sub", &DP, 18}, {"subs", &DP, 19}, 
       	{"and", &DP, 0}, {"ands", &DP, 6}, {"bic", &DP, 33}, {"bics", &DP, 39},
	{"eor", &DP, 37}, {"eon", &DP, 4}, {"orr", &DP, 2}, {"orn", &DP, 36},
	{"movn", &DP, 6}, {"movk", &DP, 7}, {"movz", &DP, 4}, {"madd", &DP, 7},
	{"msub", &DP, 32}, 
	{"b", &BR, 3}, {"br", &BR, 4}, {"b.eq", &BR, 0}, {"b.ne", &BR, 1},
	{"b.ge", &BR, 10}, {"b.lt", &BR, 11}, {"b.gt", &BR, 12}, {"b.le", &BR, 13}, 
	{"b.al", &BR, 14}, 
	{"ldr", &Transfer, 7}, {"str", &Transfer, 7},{"nop", &Transfer, 7}}; 
	

int decode(instruction_data inst) {
	
	for (int i = 0; i < 30; i++) {
		if (strcmp(instToFunctions[i].key,inst.instruction_name) == 0) {
			instructionMaker fun = (instToFunctions[i].function);printf("checking %ld \n", &fun);

			return fun(&inst, instToFunctions[i].opcode);
		}
	}

	// it should never reach this value	
	assert(false);
	return 0;
}
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
int main(void) {
	int i = 30;
	register_id reg_id;
	reg_id.number = i;
	register_info reginfo = {GENERAL,reg_id, BIT_64};
	operand first;
	first.value.register_operand = reginfo;
	first.type = REGISTER;
	
	operand ands =  {REGISTER};
	operand op[4] = {RegisterN(30),RegisterN(0),RegisterN(3),RegisterN(15)};//{{IMMEDIATE,( 1 << 26) - 1}};
	instruction_data test = {"madd", op, 4};
	int result = decode(test);
	toBinaryPrint(result);
	return 0;
}
