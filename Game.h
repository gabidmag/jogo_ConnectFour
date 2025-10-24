#ifndef GAME_H 
#define GAME_H

// Dimensões do tabuleiro
#define BOARD_ROWS 6
#define BOARD_COLS 7

// Símbolos das peças
#define EMPTY_CELL    '.'  // Célula vazia
#define PLAYER1_PIECE 'X'  // Jogador 1
#define PLAYER2_PIECE 'O'  // Jogador 2 ou IA

// Modos de jogo possíveis
typedef enum {
    MODE_PVP,   // Jogador vs Jogador
    MODE_PVIA   // Jogador vs IA
} GameMode;

// Declarações de todas as funções do jogo
void initialize_board(char board[BOARD_ROWS][BOARD_COLS]);
void display_board(char board[BOARD_ROWS][BOARD_COLS]);
int get_human_move(char board[BOARD_ROWS][BOARD_COLS], char current_piece);
int get_ia_move(char board[BOARD_ROWS][BOARD_COLS]);
int drop_piece(char board[BOARD_ROWS][BOARD_COLS], int col, char piece);
int check_win(char board[BOARD_ROWS][BOARD_COLS], char piece);
int check_draw(char board[BOARD_ROWS][BOARD_COLS]);
void run_game();  // Função principal do jogo

#endif