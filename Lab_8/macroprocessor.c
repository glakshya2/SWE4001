#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20
#define MAX_BODY_LINES 50
#define MAX_PARAMS 3

typedef struct {
    char name[MAX_TOKEN_LEN];
    char params[MAX_PARAMS][MAX_TOKEN_LEN];
    char body[MAX_BODY_LINES][MAX_LINE_LENGTH];
    int body_line_count;
} Macro;

Macro macro;

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

void expandMacro(FILE* out, const char* line) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    char params[MAX_PARAMS][MAX_TOKEN_LEN];
    split(line, ' ', tokens);
    split(tokens[1], ',', params);
    fprintf(out, "**expanding** %s\n", line);
    for (int i = 0; i < macro.body_line_count; i++) {
        split(macro.body[i], ' ', tokens);
        for (int j = 0; j < MAX_TOKENS && tokens[j][0] != '\0'; j++) {
            if (strcmp(tokens[j], macro.params[0]) == 0) {
                fprintf(out, "%s ", params[0]);
            } else if (strcmp(tokens[j], macro.params[1]) == 0) {
                fprintf(out, "%s ", params[1]);
            } else if (strcmp(tokens[j], macro.params[2]) == 0) {
                fprintf(out, "%s ", params[2]);
            } else {
                fprintf(out, "%s ", tokens[j]);
            }
        }
        fprintf(out, "\n");
    }
}

int main() {
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    macro.body_line_count = 0;
    int in_macro_definition = 0;

    while (fgets(line, sizeof(line), input_file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (!in_macro_definition && strstr(line, "MACRO") != NULL) {
            in_macro_definition = 1;
            split(line, ' ', tokens);
            strcpy(macro.name, tokens[0]);
            split(tokens[2], ',', macro.params);
        } else if (in_macro_definition) {
            if (strstr(line, "MEND") != NULL) {
                in_macro_definition = 0;
            } else {
                strcpy(macro.body[macro.body_line_count++], line);
            }
        } else if (strlen(macro.name) > 0 && strstr(line, macro.name) != NULL) {
            expandMacro(output_file, line);
        } else {
            fprintf(output_file, "%s\n", line);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
