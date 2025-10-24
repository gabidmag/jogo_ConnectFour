#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// Limpa a tela do terminal (compatível Windows/Linux)
void clear_screen(void) {
#ifdef _WIN32
    system("cls");        // Comando para Windows
#else
    system("clear");      // Comando para Linux/Mac
#endif
}

// Gera número aleatório entre min e max (inclusive)
int rand_between(int min, int max) {
    if (max <= min) return min;                    // Evita divisão por zero
    return (rand() % (max - min + 1)) + min;       // Gera número no intervalo
}

// Pausa o programa até usuário pressionar Enter
void press_enter_to_continue(void) {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n');  // Espera até que Enter seja pressionado
}

// Avalia o tabuleiro - conta quantas peças o jogador tem
int evaluate_board(int **board, int rows, int cols, int player) {
    int score = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == player) {  // Se célula tem peça do jogador
                score++;                  // Incrementa pontuação
            }
        }
    }
    return score;  // Retorna total de peças do jogador
}

// Imprime uma linha divisória com caractere específico
void print_divider(char c, int length) {
    for (int i = 0; i < length; i++) {
        putchar(c);  // Imprime o caractere repetidamente
    }
    putchar('\n');   // Quebra de linha no final
}