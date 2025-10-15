#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int rand_between(int min, int max) {
    if (max <= min) return min;
    return (rand() % (max - min + 1)) + min;
}

void press_enter_to_continue(void) {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n');
}

int evaluate_board(int **board, int rows, int cols, int player) {
    int score = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == player) {
                score++;
            }
        }
    }
    return score;
}

void print_divider(char c, int length) {
    for (int i = 0; i < length; i++) {
        putchar(c);
    }
    putchar('\n');
}