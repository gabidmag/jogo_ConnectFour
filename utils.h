#ifndef UTILS_H      // Proteção contra inclusão múltipla
#define UTILS_H
#include <stdbool.h> // Para tipo bool (true/false)

// Declarações das funções utilitárias
void clear_screen(void);                              // Limpa terminal
int rand_between(int min, int max);                   // Número aleatório
void press_enter_to_continue(void);                   // Pausa para Enter
int evaluate_board(int **board, int rows, int cols, int player);  // Avalia tabuleiro
void print_divider(char c, int length);               // Linha divisória

#endif