#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

void clear_screen(void);
int rand_between(int min, int max);
void press_enter_to_continue(void);
int evaluate_board(int **board, int rows, int cols, int player);
void print_divider(char c, int length);

#endif