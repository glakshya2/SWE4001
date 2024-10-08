#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

void split(char* str, char delimiter, char tokens[15][MAX_LINE_LENGTH]) {
    int tokenIndex = 0;
    int charIndex = 0;
    for (int i = 0; i < 15; i++) {
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

int main(void) {
    FILE* input = fopen("load-input.txt", "r");
    FILE* output = fopen("load-output.txt", "w");
    int startAddress = 0, currentAddress = 0;
    char line[MAX_LINE_LENGTH];
    char tokens[15][MAX_LINE_LENGTH];
    char printLIne[MAX_LINE_LENGTH] = "";
    while(fgets(line, MAX_LINE_LENGTH, input)) {
        line[strlen(line) - 1] = '\0';
        split(line, '\t', tokens);
        char type = tokens[0][0];
        if (type == 'H') {
            startAddress = (int) strtol(tokens[2], NULL, 16);
            currentAddress = startAddress;
        } else if (type == 'T') {
            int tokenIndex = 3;
            while(tokens[tokenIndex][0] != '\0') {
                
            }
        } else if (type == 'E') {
            break;
        }
    }
    fclose(input);
    fclose(output);
}