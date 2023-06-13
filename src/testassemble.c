
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
    
    for (int i = 0; i < NTESTS; i++) {
        line_data line = process_line(files[i].input);
    int result = decodeline(line);
    if (files[i].output == result) {
        printf("Test has passed\n");
    } else {
        printf("FAILED test expected: %x obtained %x\n", files[i].output, result);
    }
    }
    
    return 0;

}

//
// Created by dn622 on 12/06/23.
//
