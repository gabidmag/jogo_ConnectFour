#include <stdio.h>
#include "board.h"

void init_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = ' ';
}

void print_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++)
            printf(" %c |", board[i][j]);
        printf("\n");
    }
    printf("  1   2   3   4   5   6   7\n");
}

int drop_piece(char board[ROWS][COLS], int col, char piece) {
    if (col < 0 || col >= COLS) return 0;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == ' ') {
            board[i][col] = piece;
            return 1;
        }
    }
    return 0;
}

int check_full(char board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++)
        if (board[0][j] == ' ') return 0;
    return 1;
}
