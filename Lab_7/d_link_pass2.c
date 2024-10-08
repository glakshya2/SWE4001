#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS 15
#define MAX_TOKEN_LEN 100
#define MAX_LINE_LEN 100
#define MAX_PROGS 15

struct defined_vars {
    char name[MAX_TOKEN_LEN];
    int addr;
};

struct prog_def {
    char name[MAX_TOKEN_LEN];
    int startAddr;
    struct defined_vars vars[MAX_TOKENS];
};

struct prog_def program[MAX_PROGS];

void split(const char* str, char delimiter, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]) {
    int tokenIndex = 0, charIndex = 0;

    for (int i = 0; i < MAX_TOKENS; i++) {
        tokens[i][0] = '\0';
    }

    for (int i = 0; i <= strlen(str); i++) {
        if (str[i] == delimiter || str[i] == '\0') {
            tokens[tokenIndex][charIndex] = '\0';
            tokenIndex++;
            charIndex = 0;
        } else {
            tokens[tokenIndex][charIndex++] = str[i];
        }
    }
}

int findProgAddr(const char name[MAX_TOKEN_LEN]) {
    for (int i = 0; i < MAX_PROGS; i++) {
        if (strcmp(program[i].name, name) == 0) {
            return program[i].startAddr;
        }
    }
    return -1;
}

int findSymbolAddr(const char name[MAX_TOKEN_LEN]) {
    for (int i = 0; i < MAX_PROGS; i++) {
        for (int j = 0; j < MAX_TOKENS; j++) {
            if (strcmp(program[i].vars[j].name, name) == 0) {
                return program[i].vars[j].addr;
            }
        }
    }
    return -1;
}

void processFile(FILE* inputFile, FILE* outputFile) {
    char line[MAX_LINE_LEN];
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int index = 1;

    while (fgets(line, MAX_LINE_LEN, inputFile)) {
        line[strcspn(line, "\n")] = '\0';
        split(line, '^', tokens);

        if (tokens[0][0] == 'H') { 
            fprintf(outputFile, "%d\t%s\t%04X\n", index, tokens[1], findProgAddr(tokens[1]));
            index++;
        } else if (tokens[0][0] == 'R') {
            for (int i = 1; i < MAX_TOKENS; i++) {
                if (tokens[i][0] == '\0') break;
                fprintf(outputFile, "%d\t%s\t%04X\n", index, tokens[i], findSymbolAddr(tokens[i]));
                index++;
            }
        }
    }
}

int main(void) {
    FILE* in = fopen("pass1_out.txt", "r");
    FILE* p_a = fopen("prog-a.txt", "r");
    FILE* p_b = fopen("prog-b.txt", "r");
    FILE* p_c = fopen("prog-c.txt", "r");
    FILE* o_a = fopen("out-a.txt", "w");
    FILE* o_b = fopen("out-b.txt", "w");
    FILE* o_c = fopen("out-c.txt", "w");

    char line[MAX_LINE_LEN];
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int programIndex = -1;
    int varsIndex = 0;

    while (fgets(line, MAX_LINE_LEN, in)) {
        line[strcspn(line, "\n")] = '\0';
        split(line, '\t', tokens);

        if (strlen(tokens[0]) != 0) {
            programIndex++;
            strcpy(program[programIndex].name, tokens[0]);
            varsIndex = 0;
            program[programIndex].startAddr = strtol(tokens[2], NULL, 16);
        } else if (strlen(tokens[1]) != 0) {
            strcpy(program[programIndex].vars[varsIndex].name, tokens[1]);
            program[programIndex].vars[varsIndex].addr = strtol(tokens[2], NULL, 16);
            varsIndex++;
        }
    }
    fclose(in);

    processFile(p_a, o_a);
    processFile(p_b, o_b);
    processFile(p_c, o_c);

    fclose(p_a);
    fclose(p_b);
    fclose(p_c);
    fclose(o_a);
    fclose(o_b);
    fclose(o_c);

    return 0;
}
