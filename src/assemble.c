#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#define MAX_INSTRUCTIONS 100

void writeFile(int32_t *arr, char *file) {
  FILE *file_out = fopen(file, "wb");
  fwrite(arr, sizeof(int32_t), MAX_INSTRUCTIONS, file_out);
  fclose(file_out);
}

int main(int argc, char **argv) {
  if (argc == 3) {
    // pass argv[1], argv[2] into assembler input
  } else {
    printf("Please enter exactly 2 command line arguments in the format <file_in> <file_out>\n");
  }

  // read file


  // go over the file


  //write into a binary file
}
