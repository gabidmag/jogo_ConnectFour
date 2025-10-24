#ifndef BOARD_H  // Protege contra inclusão múltipla
#define BOARD_H

// Constantes do tabuleiro
#define ROWS 6    // 6 linhas no tabuleiro
#define COLS 7    // 7 colunas no tabuleiro

// Declarações das funções do tabuleiro
void init_board(char board[ROWS][COLS]);     // Inicializa tabuleiro vazio
void print_board(char board[ROWS][COLS]);    // Exibe tabuleiro na tela
int drop_piece(char board[ROWS][COLS], int col, char piece);  // Coloca peça
int check_full(char board[ROWS][COLS]);      // Verifica se tabuleiro está cheio

#endif