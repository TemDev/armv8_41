
#include <stdio.h>
#include <string.h>

void convertInstruction(char* instruction) {
    char temp[50];
    char* token;
    char* operands[3];

    // Copy the instruction string to a temporary buffer
    strcpy(temp, instruction);

    // Tokenize the instruction using space as the delimiter
    token = strtok(temp, " ");
    int i = 0;

    // Store the tokens (instruction name and operands) in the operands array
    while (token != NULL) {
        operands[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    if ((instruction << 31) == 1) {
        mode = xzr; // 64-bit mode
    } else {
        mode = wzr; // 32-bit mode
    }

    // Check if the instruction is "cmp" and has two operands
    if (strcmp(operands[0], "cmp") == 0) {
        // Construct the "subs" instruction using the operands
        sprintf(instruction, "subs, %s, %s, %s", mode, operands[1], operands[2]);

    } else if (strcmp(operands[0], "cmn") == 0) {
        // Construct the "adds" instruction using the operands
        sprintf(instruction, "adds, %s, %s, %s", mode, operands[1], operands[2]);

    } else if (strcmp(operands[0], "neg") == 0) {
        // Construct the "sub" instruction using the operands
        sprintf(instruction, "sub, %s, %s, %s", operands[1], mode, operands[2]);

    } else if (strcmp(operands[0], "negs") == 0) {
        // Construct the "subs" instruction using the operands
        sprintf(instruction, "subs, %s, %s, %s", operands[1], mode, operands[2]);

    } else if (strcmp(operands[0], "tst") == 0) {
        // Construct the "ands" instruction using the operands
        sprintf(instruction, "ands, %s, %s, %s", mode, operands[1], operands[2]);

    } else if (strcmp(operands[0], "mvn") == 0) {
        // Construct the "orn" instruction using the operands
        sprintf(instruction, "orn, %s, %s, %s", operands[1], mode, operands[2]);

    } else if (strcmp(operands[0], "mov") == 0) {
        // Construct the "orr" instruction using the operands
        sprintf(instruction, "orr %s, %s, %s", operands[1], mode, operands[2]);

    } else if (strcmp(operands[0], "mul") == 0) {
        // Construct the "madd" instruction using the operands
        sprintf(instruction, "madd, %s, %s, %s", operands[1], operands[2], mode);

    } else if (strcmp(operands[0], "mneg") == 0) {
        // Construct the "msub" instruction using the operands
        sprintf(instruction, "msub, %s, %s, %s", operands[1], operands[2], mode);

    } else {
        printf("Invalid instruction format or unsupported instruction.\n");
    }
}

int main() {
    char instruction[100];

    fgets(instruction, sizeof(instruction), stdin);
    instruction[strcspn(instruction, "\n")] = '\0';  // Remove the newline character

    convertInstruction(instruction);

    printf("Converted instruction: %s\n", instruction);

    return 0;
}
