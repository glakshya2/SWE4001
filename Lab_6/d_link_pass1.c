#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS 15
#define MAX_TOKEN_LEN 100

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

void processFile(FILE* inputFile, FILE* outputFile, int* prevEnd) {
    char line[100], tokens[MAX_TOKENS][MAX_TOKEN_LEN], name[20];
    int startAddress, length;
    char type;

    while (fgets(line, sizeof(line), inputFile)) {
        line[strcspn(line, "\n")] = '\0';
        split(line, '^', tokens);
        type = tokens[0][0];

        if (type == 'H') {
            strcpy(name, tokens[1]);
            length = strtol(tokens[3], NULL, 16);
            startAddress = strtol(tokens[2], NULL, 16) + *prevEnd;
            *prevEnd = startAddress + length;
            fprintf(outputFile, "%s\t\t%04X\t%03X\n", name, startAddress, length);
        } else if (type == 'D') {
            for (int i = 1; i < MAX_TOKENS - 1; i += 2) {
                if (tokens[i][0] == '\0') {
                    break;
                }
                fprintf(outputFile, "\t%s\t%04X\n", tokens[i], strtol(tokens[i + 1], NULL, 16) + startAddress);
            }
        } else if (type == 'M' || type == 'E' || type == 'R' || type == 'T') {
            break;
        }
    }
}

int main(void) {
    FILE* in1 = fopen("prog-a.txt", "r");
    FILE* in2 = fopen("prog-b.txt", "r");
    FILE* in3 = fopen("prog-c.txt", "r");
    FILE* out = fopen("out.txt", "w");

    if (!in1 || !in2 || !in3 || !out) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int prevEnd = 0x4000;

    processFile(in1, out, &prevEnd);
    processFile(in2, out, &prevEnd);
    processFile(in3, out, &prevEnd);

    fclose(in1);
    fclose(in2);
    fclose(in3);
    fclose(out);

    return 0;
}
