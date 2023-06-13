#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "AssemblerInput.h"
#define MAX_INSTRUCTIONS 100

typedef struct label_list{
  char* label;
  int address;
} label_list;

void writeFile(int32_t *arr, char *file) {
  FILE *file_out = fopen(file, "wb");
  fwrite(arr, sizeof(int32_t), MAX_INSTRUCTIONS, file_out);
  fclose(file_out);
}

int main(int argc, char **argv) {
  if (argc == 3) {
    line_data line_tokens[MAX_INSTRUCTIONS];
    process_input(argv[1], *line_tokens);

    label_list label_list_actual[MAX_INSTRUCTIONS];
    int count = 0;
    int count_address = 0;
    for(int i = 0; line_tokens[i].type; i++) {
      if (line_tokens[i].type == LABEL) {
        label_list_actual[count].label = line_tokens[i].contents.label_name;
        label_list_actual[count].address = count_address;
        count++;
      } else {
        count_address += 4;
      }
    }

    for (int i = 0; line_tokens[i].type; i++) {
        if (line_tokens[i].type == INSTRUCTION) {
            for (int j = 0; j < line_tokens[i].contents.instruction.no_operands; j++) {
                if ((line_tokens[i].contents.instruction.operands[j].type == LITERAL) && line_tokens[i].contents.instruction.operands[j].value.label_name) {
                    int label_value;
                    int k = 0;
                    while(label_list_actual[k].label != line_tokens[i].contents.instruction.operands[j].value.label_name) {k++;}
                    label_value = label_list_actual[k].address;
                    line_tokens[i].contents.instruction.operands[j].value.immediate = label_value;
                    line_tokens[i].contents.instruction.operands[j].type = IMMEDIATE;
                }
            }
        }
    }
    // pass argv[1], argv[2] into assembler input
  } else {
    printf("Please enter exactly 2 command line arguments in the format <file_in> <file_out>\n");
  }

  // read file


  // go over the file


  //write into a binary file
}
