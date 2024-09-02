#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 10
#define MAX_OPCODE_LENGTH 10
#define MAX_OPERAND_LENGTH 10

struct Symbol {
    char name[MAX_LABEL_LENGTH];
    int address;
};

void split(char* str, char delimiter, char tokens[3][MAX_LINE_LENGTH]) {
    int tokenIndex = 0;
    int charIndex = 0;
    for (int i = 0; i < 3; i++) {
        tokens[i][0] = '\0';
    }

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == delimiter) {
            tokens[tokenIndex][charIndex] = '\0';
            tokenIndex++;
            charIndex = 0;
        } else {
            tokens[tokenIndex][charIndex++] = str[i];
        }
    }
    tokens[tokenIndex][charIndex] = '\0';
}

int main() {
    FILE* inputFile = fopen("input.txt", "r");
    FILE* intermediateFile = fopen("intermediate.txt", "w");
    FILE* symtabFile = fopen("symtab.txt", "w");

    struct Symbol symtab[100];
    int symtabIndex = 0;

    int locctr = 0;
    int startingAddress = 0;
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_LENGTH], opcode[MAX_OPCODE_LENGTH], operand[MAX_OPERAND_LENGTH];
    char tokens[3][MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        line[strcspn(line, "\n")] = '\0';

        split(line, '\t', tokens);
        strcpy(label, tokens[0]);
        strcpy(opcode, tokens[1]);
        strcpy(operand, tokens[2]);

        if (strcmp(opcode, "START") == 0) {
            locctr = (int)strtol(operand, NULL, 16);
            startingAddress = locctr;
            fprintf(intermediateFile, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
            continue;
        }

        if (strcmp(opcode, "END") == 0) {
            fprintf(intermediateFile, "\t%s\t%s\t%s\n", label, opcode, operand);
            break;
        }

        if (label[0] != '\0' && label[0] != '.') {
            int isDuplicate = 0;
            for (int i = 0; i < symtabIndex; i++) {
                if (strcmp(symtab[i].name, label) == 0) {
                    printf("Error: Duplicate symbol %s\n", label);
                    isDuplicate = 1;
                    break;
                }
            }
            if (!isDuplicate) {
                strcpy(symtab[symtabIndex].name, label);
                symtab[symtabIndex].address = locctr;
                symtabIndex++;
            }
        }

        fprintf(intermediateFile, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        if (strcmp(opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            locctr += 3 * atoi(operand);
        } else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            locctr += strlen(operand) - 3;
        } else {
            locctr += 3;
        }
    }

    int programLength = locctr - startingAddress;
    printf("Program length: %04X bytes\n", programLength);

    for (int i = 0; i < symtabIndex; i++) {
        fprintf(symtabFile, "%04X\t%s\n", symtab[i].address, symtab[i].name);
    }

    fclose(inputFile);
    fclose(intermediateFile);
    fclose(symtabFile);

    return 0;
}
