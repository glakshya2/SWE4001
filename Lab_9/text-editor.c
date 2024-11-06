#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 100

char text[MAX_ROWS][MAX_COLS] = {{0}};  // Initialize text buffer
int cur_row = 0, cur_col = 0;
char filename[50];

void display_text();
void move_cursor(int row, int col);
void insert_char(char ch);
void backspace();
void delete_char();
void handle_key_input();
void save_file();
void load_file();

void display_text() {
    system("cls");
    printf("Editing '%s' (ESC to save and exit)\n", filename);
    for (int i = 0; i <= cur_row; i++) {
        printf("%s\n", text[i]);
    }
    move_cursor(cur_row, cur_col);
}

void move_cursor(int row, int col) {
    COORD coord = {col, row + 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void insert_char(char ch) {
    for (int i = MAX_COLS - 1; i > cur_col; i--) {
        text[cur_row][i] = text[cur_row][i - 1];
    }
    text[cur_row][cur_col] = ch;
    cur_col++;
    display_text();
}

void backspace() {
    if (cur_col > 0) {
        for (int i = cur_col - 1; i < MAX_COLS - 1; i++) {
            text[cur_row][i] = text[cur_row][i + 1];
        }
        cur_col--;
        display_text();
    }
}

void delete_char() {
    for (int i = cur_col; i < MAX_COLS - 1; i++) {
        text[cur_row][i] = text[cur_row][i + 1];
    }
    display_text();
}

void handle_key_input() {
    int ch;
    while (1) {
        ch = _getch();
        if (ch == 27) {
            save_file();
            break;
        } else if (ch == 13) {
            cur_row++;
            cur_col = 0;
        } else if (ch == 8) {
            backspace();
        } else if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case 72:
                    if (cur_row > 0) cur_row--;
                    break;
                case 80:
                    if (cur_row < MAX_ROWS - 1) cur_row++;
                    break;
                case 75:
                    if (cur_col > 0) cur_col--;
                    break;
                case 77:
                    if (cur_col < MAX_COLS - 1) cur_col++;
                    break;
                case 83:
                    delete_char();
                    break;
            }
        } else if (ch >= 32 && ch <= 126) {
            insert_char((char)ch);
        }
        display_text();
    }
}

void save_file() {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i <= cur_row; i++) {
        fprintf(file, "%s\n", text[i]);
    }
    fclose(file);
    printf("File '%s' saved successfully.\n", filename);
}

void load_file() {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char line[MAX_COLS];
        cur_row = 0;
        while (fgets(line, MAX_COLS, file) != NULL && cur_row < MAX_ROWS) {
            strcpy(text[cur_row], line);
            text[cur_row][strcspn(text[cur_row], "\n")] = 0;
            cur_row++;
        }
        cur_row--;
        fclose(file);
        printf("File '%s' loaded successfully.\n", filename);
    } else {
        printf("File '%s' does not exist. Starting with an empty file.\n", filename);
    }
}

int main() {
    printf("Enter filename to edit: ");
    scanf("%s", filename);
    load_file();
    display_text();
    handle_key_input();
    return 0;
}
