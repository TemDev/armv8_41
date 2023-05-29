#include <stdio.h>
#include <stdlib.h>
#define MEM_SIZE 16394
#define HLT 0x8a000000

int main(int argc, char** argv) {
        // checks whether there is exactly one output
        if( argc != 2 ) {
                fprintf( stderr, "Usage: cat filename!\n" );
                exit(1);
        }
        FILE *fp = fopen(argv[1], "rb");
        int *memory = (int*) malloc(MEM_SIZE * sizeof(int));
        int count = 0;
        while (!feof(fp) && count < 3) {
                fread(&memory[count], sizeof(int), 1, fp);
                printf("%.8x \n", memory[count]);
                count++;
        }

        int inst = memory[0];
        while (inst == HLT ) { // it is supposed to be != but in this case is in infinite loop
                // process this instruction
                // update the CompState
                // and memory corresponding to the
                // next insturction
                // get the next instruction back
                // to inst value
        }


        FILE *file = fopen("hello.out", "w");

        for (int i = 0; i < MEM_SIZE; i++){
                if (memory[i] != 0) {
                fprintf(file,"0x%.8x : 0x%.8x \n", i << 2, memory[i]);                                                  }
        }
        //closes opened files and frees the memory
        free(memory);
        fclose(fp);
        fclose(file);

        return EXIT_SUCCESS;
}
