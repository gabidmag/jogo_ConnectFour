#ifndef GAME_H
#define GAME_H

#define BOARD_ROWS 6
#define BOARD_COLS 7

#define EMPTY_CELL    '.'
#define PLAYER1_PIECE 'X'
#define PLAYER2_PIECE 'O'

typedef enum {
    MODE_PVP,
    MODE_PVIA
} GameMode;

void initialize_board(char board[BOARD_ROWS][BOARD_COLS]);
void display_board(char board[BOARD_ROWS][BOARD_COLS]);
int get_human_move(char board[BOARD_ROWS][BOARD_COLS], char current_piece);
int get_ia_move(char board[BOARD_ROWS][BOARD_COLS]);
int drop_piece(char board[BOARD_ROWS][BOARD_COLS], int col, char piece);
int check_win(char board[BOARD_ROWS][BOARD_COLS], char piece);
int check_draw(char board[BOARD_ROWS][BOARD_COLS]);
void run_game();

#endif
