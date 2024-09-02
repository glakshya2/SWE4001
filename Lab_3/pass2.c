#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 10
#define MAX_OPCODE_LENGTH 10
#define MAX_OPERAND_LENGTH 10

struct Opcode {
    char name[MAX_OPCODE_LENGTH];
    int code;
};

struct Opcode optab[] = {{"LDA", 0x000000}, {"ADD", 0x1A0000}, {"SUB", 0x1C0000}, {"STA", 0x0C0000}}; 

struct Symbol {
    char name[MAX_LABEL_LENGTH];
    int address;
};

struct Intermediate {
    int address;
    char label[MAX_LABEL_LENGTH];
    char opcode[MAX_OPCODE_LENGTH];
    char operand[MAX_OPERAND_LENGTH];
    int objcode;
};

void split(char* str, char delimiter, char tokens[4][MAX_LINE_LENGTH]) {
    int tokenIndex = 0;
    int charIndex = 0;
    for (int i = 0; i < 4; i++) {
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

int findOpcode(char* opcode) {
    int arraySize = sizeof(optab) / sizeof(optab[0]);
    for (int i = 0; i < arraySize; i++) {
        if (strcmp(optab[i].name, opcode) == 0) {
            return optab[i].code;
        }
    }
    return -1;
}

int findOpernadAddress(char* label, struct Symbol symtab[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        if (strcmp(symtab[i].name, label) == 0) {
            return symtab[i].address;
        }
    }
    return -1;
}

int stringToHexInt(const char* str) {
    int result = 0;
    while(*str) {
        result = (result << 8) | (unsigned char)(*str);
        str++;
    }
    return result;
}

void copyToTokenizedProgram(struct Intermediate tokenizedProgram[], int index, int address, char* label, char* opcode, char* operand, int objCode) {
    tokenizedProgram[index].address = address;
    strcpy(tokenizedProgram[index].label, label);
    strcpy(tokenizedProgram[index].opcode, opcode);
    strcpy(tokenizedProgram[index].operand, operand);
    tokenizedProgram[index].objcode = objCode;
}

int main() {
    FILE* symtabFile = fopen("symtab.txt", "r");
    FILE* intermediateFile = fopen("intermediate.txt", "r");
    FILE* objcodeFile = fopen("pgm-objcode.txt", "w");
    FILE* objpgmFile = fopen("objpgm.txt", "w");
    struct Symbol symtab[100];
    int symtabIndex = 0;

    struct Intermediate tokenizedProgram[100];
    int tokenizedProgramIndex = 0;

    char line[MAX_LINE_LENGTH], label[MAX_LABEL_LENGTH];
    char tokens[4][MAX_LINE_LENGTH];

    int programLength = 0;

    while(fgets(line, MAX_LINE_LENGTH, symtabFile)) {
        line[strcspn(line, "\n")] = '\0';
        split(line, '\t', tokens);
        symtab[symtabIndex].address = (int)strtol(tokens[0], NULL, 16);
        strcpy(symtab[symtabIndex].name, tokens[1]);
        symtabIndex++;
    }
    int startingAddress, address;

    for (int i = 0; i < MAX_LABEL_LENGTH; i++) {
        label[i] = '\0';
    }

    char programName[MAX_LABEL_LENGTH], opcode[MAX_OPCODE_LENGTH], operand[MAX_OPERAND_LENGTH];
    while(fgets(line, MAX_LINE_LENGTH, intermediateFile)) {
        line[strcspn(line, "\n")] = '\0';
        split(line, '\t', tokens);
        address = (int)strtol(tokens[0], NULL, 16);
        strcpy(label, tokens[1]);
        strcpy(opcode, tokens[2]);
        strcpy(operand, tokens[3]);
        if (strcmp(opcode, "START") == 0) {
            copyToTokenizedProgram(tokenizedProgram, tokenizedProgramIndex++, address, label, opcode, operand, 0);
            continue;
        }

        if (strcmp(opcode, "END") == 0) {
            copyToTokenizedProgram(tokenizedProgram, tokenizedProgramIndex++, address, label, opcode, operand, 0);
            break;
        }

        int objCode = findOpcode(opcode);
        
        if (objCode != -1) {
            int operandAddress = findOpernadAddress(operand, symtab, sizeof(symtab) / sizeof(symtab[0]));
            if (operandAddress != -1) {
                copyToTokenizedProgram(tokenizedProgram, tokenizedProgramIndex++, address, label, opcode, operand, objCode + operandAddress);
                programLength += 3;
            }
        } else if (strcmp(opcode, "BYTE") == 0) {
            char actualString[100];
            strncpy(actualString, operand + 2, strlen(operand) - 3);
            copyToTokenizedProgram(tokenizedProgram, tokenizedProgramIndex++, address, label, opcode, operand, stringToHexInt(actualString));
            programLength += strlen(actualString);
        } else if (strcmp(opcode, "WORD") == 0) {
            copyToTokenizedProgram(tokenizedProgram, tokenizedProgramIndex++, address, label, opcode, operand, atoi(operand));
            programLength += 3;
        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            copyToTokenizedProgram(tokenizedProgram, tokenizedProgramIndex++, address, label, opcode, operand, 0);
            if (strcmp(opcode, "RESW") == 0) {
                programLength += atoi(operand) * 3;
            } else {
                programLength += atoi(operand);
            }
        }        
    }

    fclose(symtabFile);
    fclose(intermediateFile);

    for (int i = 0; i < tokenizedProgramIndex; i++) {
        if (strcmp(tokenizedProgram[i].opcode, "END") == 0) {
            fprintf(objcodeFile, "\t%s\t%s\t%s", tokenizedProgram[i].label, tokenizedProgram[i].opcode, tokenizedProgram[i].operand);
        } else {
            fprintf(objcodeFile, "%04X\t%s\t%s\t%s", tokenizedProgram[i].address, tokenizedProgram[i].label, tokenizedProgram[i].opcode, tokenizedProgram[i].operand);
        }
        if (strcmp(tokenizedProgram[i].opcode, "START") == 0 || strcmp(tokenizedProgram[i].opcode, "RESW") == 0 || strcmp(tokenizedProgram[i].opcode, "RESB") == 0 || strcmp(tokenizedProgram[i].opcode, "END") == 0) {
            fprintf(objcodeFile, "\n");
        } else {
            fprintf(objcodeFile, "\t%06X\n", tokenizedProgram[i].objcode);
        }
    }
    fclose(objcodeFile);

    char name[7];
    int passed0 = 0;
    for (int i = 0; i < 6; i++) {
        if (passed0 == 0) {
            name[i] = tokenizedProgram[0].label[i];
            if (name[i] == '\0') {
                passed0 = 1;
            }
        } else {
            name[i] = ' ';
        }
    }
    name[6] = '\0';

    fprintf(objpgmFile, "H\t%s\t%06X\t%06X\n", name, (int) strtol(tokenizedProgram[0].operand, NULL, 16), programLength);
    int recordSize = -1;
    int recordStart = -1;
    char objcodes[61];
    int objcodesIndex = 0;
    for (int i = 0; i < tokenizedProgramIndex; i++) {
        if (strcmp(tokenizedProgram[i].opcode, "START") == 0 || strcmp(tokenizedProgram[i].opcode, "RESW") == 0 || strcmp(tokenizedProgram[i].opcode, "RESB") == 0) {
            continue;
        }
        if (strcmp(tokenizedProgram[i].opcode, "END") == 0) {
            break;
        }
        if ((recordSize != -1 && (strcmp(tokenizedProgram[i + 1].opcode, "END") == 0 || strcmp(tokenizedProgram[i + 1].opcode, "RESW") == 0 || strcmp(tokenizedProgram[i + 1].opcode, "RESB") == 0)) || recordSize == 0x1E) {
            objcodes[objcodesIndex] = '\0';
            fprintf(objpgmFile, "T\t%06X\t%02X\t%s\n", recordStart, recordSize, objcodes);
            recordSize = -1;
            recordStart = -1;
            objcodes[0] = '\0';
            objcodesIndex = 0;
        }
        if (recordSize == -1) {
            recordSize += 4;
        } else {
            recordSize += 3;
        }
        if (recordStart == -1) {
            recordStart = tokenizedProgram[i].address;
        }
        sprintf(objcodes + objcodesIndex, "%06X", tokenizedProgram[i].objcode);
        objcodesIndex += 6;
    }
    fprintf(objpgmFile, "E\t%06X\n", (int) strtol(tokenizedProgram[0].operand, NULL, 16));
    fclose(objpgmFile);
}