#include <stdio.h>
#include "board.h"

// Inicializa o tabuleiro com espaços vazios
void init_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = ' ';  // Cada célula começa vazia
}

// Exibe o tabuleiro formatado na tela
void print_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++)
            printf(" %c |", board[i][j]);  // Mostra cada célula
        printf("\n");
    }
    printf("  1   2   3   4   5   6   7\n");  // Números das colunas
}

// Tenta colocar uma peça na coluna especificada
int drop_piece(char board[ROWS][COLS], int col, char piece) {
    if (col < 0 || col >= COLS) return 0;  // Coluna inválida
    
    // Procura de BAIXO para CIMA na coluna
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == ' ') {  // Encontrou espaço vazio
            board[i][col] = piece;   // Coloca a peça
            return 1;                // Sucesso
        }
    }
    return 0;  // Coluna cheia
}

// Verifica se o tabuleiro está completamente cheio
int check_full(char board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++)
        if (board[0][j] == ' ') return 0;  // Se topo tem espaço, não está cheio
    return 1;  // Todas as colunas do topo estão preenchidas
}