
#include "AssemblerDecoder.c"


int main(void) {
    char operation[] = "adds x10, x12, #155, ror #14";
    line_data line = process_line(operation);
    decodeline(line);

    return 0;

}

//
// Created by dn622 on 12/06/23.
//
