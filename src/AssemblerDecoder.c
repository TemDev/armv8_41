// Handles the decoding of the instructions

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "bitwise.c"
#include "AssemblerDecoder.h"
#include "Aliases.h"

// return a string that is a binary representation of a number

// Function that gets register number from a character index.
char getRegisterNumber(unsigned char index, instruction_data* inst) {
	//enures that index of the operand to get is less than size
	assert(inst -> no_operands > index);
	register_info reg = inst -> operands[index].value.register_operand;
	if (reg.type == GENERAL) {
		// returns a correct number
		return reg.id.number;
	} else if (reg.type == SPECIAL) {
		// any special register is encoded as 31
		return 31;
	} else {
		// not a register provided
		printf("Register expected other type provided\n");
		exit(1);
	}
	
}

// branch instructions depend on the type of branch
// if it is a conditional branch it also depends on a condtions
// a condition corresponds to last 4 bits in the instructions
// which are already encoded in the opcode parameter

// opcode values 3 and 4 are not used by conditional branches
// so they are used to identify register and uncoditional branches
int32_t BR(instruction_data *inst, char opcode) {
	int32_t temp = 0;
	assert(inst-> no_operands > 0);
	if (opcode == 3) {
		int32_t simm26 = inst -> operands[0].value.immediate;
	       temp = (5 << 26) + ((simm26 & ((1 << 26)-1)));	
	} else if (opcode == 4) {
		// register offset
		 char xn = getRegisterNumber(0, inst);
		 temp = (107 << 25) + (31 << 16) +  (xn << 5);
	}
	else {
		// conditional branches
		int32_t simm19 = inst -> operands[0].value.immediate;
		temp = (21 << 26) + (((( 1 << 19 ) - 1) & simm19) << 5) + (15 & opcode);
	}	
	return temp;
}

// Function that determines what type of address is presented
// opcode just represents load or store value
int32_t Transfer(instruction_data *inst, char opcode){
	offset_type type = inst -> operands[1].value.address1.address_type;
	bool load  = (inst -> operands[1].type != ADDRESS);
	int temp = 0;
	// gets the size bit
	char sf = (inst -> operands[0].value.register_operand.size == BIT_64)? 1:0;
	// bits that are going to be encoded into the instruction
	char U = (type == UNSIGNED)? 1:0;
	char I = (type == PRE)? 1:0;
	char L = opcode & 1;
	
	if (load) { 
		// processes load literal
		temp = ((1 & sf) << 3) + 3;
		// gets the immediate value from the operand
		int32_t simm19 = ((inst -> operands[1].value.immediate) & ((1 << 19)-1));
		// assembles the instruction correctly
		temp = (temp <<27) + (simm19 << 5);
	} else {
		// processing of all other types of addressing modes
		temp = ((1 & sf) << 3) + 23;
		int32_t offset = 0;
		// determines what value shoud the offset be
		if (type == UNSIGNED) {
			offset = inst -> operands[1].value.address1.operand2.immediate_value & ((1 << 12) - 1);
			offset = (offset >> 3);
		} else if (type == REG_SHIFT) {
			// proccess special registers
			char xm = (inst -> operands[1].value.address1.operand1.type == SPECIAL)? 
			31 :inst -> operands[1].value.address1.operand2.register_value.id.number;
			offset = (1 << 11) + (xm << 6) + 26;
		} else {
			// PRE and POST Increment addressing modes
			int32_t simm9 = inst -> operands[1].value.address1.operand2.immediate_value & ((1 << 9) - 1);
			offset = (simm9 << 2 )+ ((I & 1) << 1) + 1;
		}
// process special registers if needed
		char xn = (inst -> operands[1].value.address1.operand1.type == SPECIAL)? 
		31 : inst -> operands[1].value.address1.operand1.id.number;
		temp = (temp << 27) +  ((1 & U) << 24)  + ((1 & L) << 22) + (offset << 10) + (xn << 5);
	
	}
	char rt = getRegisterNumber(0, inst);
	temp += rt;

	
    return temp;
}



// Function that does data processing
//opcode has the following structure 
//opcode |N|R|I|opc|
//       |1|2|1|  2| these are sizes in bits of each part
// N says whether it is negated used for multiply and logical instructions 1 is negated, 0 is not
// R shows which type of instruction it is if it is a register data processing
// 1 is multiply, 0 is a logical instruction, 2 is an arithmetic instruction
// I shows which immediate data processing operation it is 
// 0 is arithmetic instruction and 1 is a wide move
// opc is a decoding that is put into the instruction(given in the spec)
// also it represents opi and opc as they are named differently in immediate and register DP
// Encodings for each functions name
// for instructions that do not have certain values 
// for example movz, it does not have an R parameter
// therefore does not depend on R, we put 0 for all parameters that it does not depend on
// add  N:0 R:01 I:0 opc:00
// adds N:0 R:01 I:0 opc:01
// sub  N:0 R:01 I:0 opc:10
// subs N:0 R:01 I:0 opc:11
// and  N:0 R:00 I:0 opc:00
// ands N:0 R:00 I:0 opc:11
// bic  N:1 R:00 I:0 opc:00
// bics N:1 R:00 I:0 opc:11
// eor  N:0 R:00 I:0 opc:10
// eon  N:1 R:00 I:0 opc:10
// orr  N:0 R:00 I:0 opc:01
// orn  N:1 R:00 I:0 opc:01
// movn N:0 R:00 I:1 opc:00
// movz N:0 R:00 I:1 opc:10
// movk N:0 R:00 I:1 opc:11
// madd N:0 R:01 I:0 opc:00
// msub N:1 R:01 I:0 opc:00
// all values are converted into denary
int32_t DP(instruction_data *inst, char opcode){
	//determines wheteher it is register or immediate type 
    bool isReg = (REGISTER == (inst -> operands[2].type)) &&
            (REGISTER == (inst -> operands[1].type));
	//determines the size
	bool is64 = BIT_64 == (inst -> operands[0].value.register_operand.size);
	int32_t temp;
	char opc, I, R, N, shift, operand;
	opc = 3 & opcode;// get last 2 bits;
	I =( opcode >> 2) & 1;
	R = (opcode >> 3) & 3;
	N = (opcode >> 5) & 1;
	
	// checks whether it is Bit Logic, Arithmetic or Multiply;
	temp = (is64)? 4: 0;
    temp += opc;

	if (isReg) {
		//adds register instruction opcode
        temp = (temp << 4) + 5;
		//gets the numbers associated with registers
		char rm = getRegisterNumber(2, inst); 
		char rn = getRegisterNumber(1, inst); 
		char rd = getRegisterNumber(0, inst);
		

        if (R == 1) {
            char ra = getRegisterNumber(3, inst);
            // adding an opcode for DP and an opc for multiply
			// denary values are representing bits that have to be added 
            temp = (temp << 4) + 8 + 128;
            //adding rm and x bit named N here and ra
            temp = (temp << 21) + (rm << 16) + ((N & 1) << 15) +( ra << 10) + (rn << 5) + rd;
        } else if (R == 0 || R == 2){
			// finds the shift encodings
            if (inst -> no_operands > 3) {
                shift = (char) inst -> operands[3].value.shift_operand.shift;
                operand =  inst -> operands[3].value.shift_operand.shift_amount & 63;
            } else {
				// if it does not have shift operand than it is equivalent to no shift
                shift = 0;
                operand = 0;
            }
			// adds shift to the instruction

            temp = (temp << 4) + ((shift & 3) << 1) ;
            if (R == 0) {
                temp += N & 1;
            } else {
                temp += 8;
            }
            //adding rm and operand
            temp = (temp << 21) + (rm  << 16) + (operand << 10) + (rn << 5) + rd;

        } else {
			// this means that it is a different r value, therefore invalid
            exit(1);
        }
	} else {
		// immediate instruction processing

		temp = (temp << 3) + 4;
		// adds opcode of DP immediate value
        char rd = getRegisterNumber(0, inst);

		if (I == 0) { // arithmetic operation
            // adds opi
			temp = (temp << 3) + 2;
            char rn = getRegisterNumber(1, inst);
            int32_t imm12 = ((inst -> operands[2].value.immediate) & ((1 << 12)-1));
            char sh = (inst -> no_operands > 3 && inst -> operands[3].value.shift_operand.shift_amount != 0)? 1 : 0;
			// the flag whether it should be shifted
			//adding sh and imm12
            temp = (temp << 23) + ((sh & 1 ) << 22) + (imm12 << 10) + (rn << 5) + rd;
			       	
	
		} else if (I == 1) {
			// wide move processing
            // adds opi to the  instructions
			temp = (temp << 3) + 5;
			// value by which it should be shifted
            char hw;
			//determines how far it should be shifted
            if (inst -> no_operands > 2) {
                hw = ((inst -> operands[2].value.shift_operand.shift_amount & 63) >> 4);
            } else {
                hw = 0;
            }
			// immediate value 
			int32_t imm16 = ((inst -> operands[1].value.immediate) & ((1 << 16)-1));
			// assembling the instruction
			temp = (temp  << 23) + (hw << 21) + (imm16 << 5) + rd;
		}
	}
	return temp;
}

// Function that handles an nop operation instruction
int32_t NOP(instruction_data *inst, char opcode) {
	return 3573751839;
}

// A map from all the instructions to a function that processes it and a specific value
// the third member of a struct is passed as an opcode
// and is a precoded value depending solely on the instruction
// the explanation how are they used is described at the start of each function referenced second
instToFunction instToFunctions[30] = {
	{"add", &DP, 16}, {"adds", &DP, 17}, {"sub", &DP, 18}, {"subs", &DP, 19}, 
       	{"and", &DP, 0}, {"ands", &DP, 3}, {"bic", &DP, 32}, {"bics", &DP, 35},
	{"eor", &DP, 2}, {"eon", &DP, 34}, {"orr", &DP, 1}, {"orn", &DP, 33},
	{"movn", &DP, 4}, {"movk", &DP, 7}, {"movz", &DP, 6}, {"madd", &DP, 8},
	{"msub", &DP, 40}, 
	{"b", &BR, 3}, {"br", &BR, 4}, {"b.eq", &BR, 0}, {"b.ne", &BR, 1},
	{"b.ge", &BR, 10}, {"b.lt", &BR, 11}, {"b.gt", &BR, 12}, {"b.le", &BR, 13}, 
	{"b.al", &BR, 14}, 
	{"ldr", &Transfer, 1}, {"str", &Transfer, 0},{"nop", &NOP, 0}}; 
	
// Function that decodes what type of instruction is presented
int32_t decode(instruction_data inst) {
	// once it finds the instruction that matches the name ir outputs the value
	for (int32_t i = 0; i < 30; i++) {
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

// Function that determines a DIRECTIVE or INSTRUCTION is presented
int32_t decodeline(line_data line) {
    switch (line.type) {
        case DIRECTIVE:
            return line.contents.directive.arg;
            break;
        case INSTRUCTION:
            return decode(convert(&line.contents.instruction));
            break;
        default:
            return 0;
    }
}