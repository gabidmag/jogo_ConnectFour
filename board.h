#ifndef BOARD_H
#define BOARD_H

#define ROWS 6
#define COLS 7

void init_board(char board[ROWS][COLS]);
void print_board(char board[ROWS][COLS]);
int drop_piece(char board[ROWS][COLS], int col, char piece);
int check_full(char board[ROWS][COLS]);

#endif
