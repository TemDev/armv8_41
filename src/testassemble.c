
#include "AssemblerInput.c"
#include "AssemblerDecoder.c"

struct pairs{
    char* input;
    int output;
};

const struct pairs makePair(char* input,char* output){
    struct pairs pair = {input,output};
    return pair;
}

void makeTest(struct pairs p) {
    
}
#define NTESTS 6



int main(void) {
    struct pairs files[NTESTS] = {{strdup("movz x1,#1"),0xd2800021},
                        {strdup("add x2,x1,#2"),0x91000822},
                        {strdup("and x0, x0, x0"),0x8a000000},
                        {strdup("movz x1,#1"),0xd2800021},
                        {strdup("movz x1,#1"),0xd2800021},
                        {strdup("movz x1,#1"),0xd2800021}};
    
    // Tests for data proccessing
    // for (int i = 0; i < NTESTS; i++) {
    //     line_data line = process_line(files[i].input);
    //     int result = decodeline(line);
    //     if (files[i].output == result) {
    //         printf("Test has passed\n");
    //     } else {
    //         printf("FAILED test expected: %x obtained %x\n", files[i].output, result);
    //     }
    // }

    // test for single data proccessing

    register_info a =RegisterZR(0).value.register_operand;// RegisterNsize(25, 'x').value.register_operand;
    register_info b = RegisterZR(0).value.register_operand;//RegisterNsize(4, 'x').value.register_operand;
    int c = 2730;
    operand address;
    address.type = ADDRESS;
    address_info info;
    info.address_type = REG;
    info.operand1 = a;
    info.operand2.register_value = b;
    //info.shift_operand = NULL;

    address.value.address1 = info;

    operand ops[2]  = {RegisterZR(0), address};
    instruction_data inst = {"str", ops, 2};
    int smth = decode(inst);
    toBinaryPrint(smth);
    printf("this is the value of the inst function %.8x\n", smth);
    
    return 0;

}

//10111 0 0 0 0 0 0101010101 0 1 11001 01101 
// Created by dn622 on 12/06/23.
//
