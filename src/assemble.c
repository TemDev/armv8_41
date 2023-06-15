#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assemble.h"
#include "AssemblerInput.h"
#include "AssemblerDecoder.h"
#define MAX_INSTRUCTIONS 100

//Structure for the list of labels
typedef struct label_list{
    char* label;
    int32_t address;
}label_list;

//Writing the processed binary output into a file
void writeFile(int32_t *arr, char *file, int size) {
  FILE *file_out = fopen(file, "wb");
  fwrite(arr, sizeof(int32_t), size, file_out);
  fclose(file_out);
}

//In this function we do the initial processing of the instructions received
int main(int argc, char **argv) {
  if (argc == 3) {
    line_data *line_tokens = (line_data*) calloc(MAX_INSTRUCTIONS,sizeof(line_data));
    int output[MAX_INSTRUCTIONS];
    int n_lines = process_input(argv[1], line_tokens);

        label_list label_list_actual[MAX_INSTRUCTIONS];
        int32_t count = 0;
        int32_t count_address = 0;
        for(int32_t i = 0; i < n_lines; i++) {
             if (line_tokens[i].type == LABEL) {
                 label_list_actual[count].label = line_tokens[i].contents.label_name;
                 label_list_actual[count].address = count_address;
                 count++;
             } else {
                 count_address += 4;
             }
        }

        count = 0;
        for (int32_t i = 0; i < n_lines; i++) {
            if (line_tokens[i].type == INSTRUCTION) {
                for (int32_t j = 0; j < line_tokens[i].contents.instruction.no_operands; j++) {
                    if ((line_tokens[i].contents.instruction.operands[j].type == LABEL_NAME) &&
                        line_tokens[i].contents.instruction.operands[j].value.label_name != NULL) {
                        int32_t label_value;
                        int32_t k = 0;
                        while (strcmp(label_list_actual[k].label,
                        line_tokens[i].contents.instruction.operands[j].value.label_name) != 0) { k++; }
                        label_value = label_list_actual[k].address;
                        line_tokens[i].contents.instruction.operands[j].value.immediate = (label_value / 4) - count;
                        line_tokens[i].contents.instruction.operands[j].type = IMMEDIATE;
                    }
                }
            }
            if (line_tokens[i].type == DIRECTIVE || line_tokens[i].type == INSTRUCTION) {
                output[count] = decodeline(line_tokens[i]);
                count++;
            }
        }
        writeFile(output, argv[2], count_address / 4);
    } else {
        printf("Please enter exactly 2 command line arguments in the format <file_in> <file_out>\n");
    }
}
