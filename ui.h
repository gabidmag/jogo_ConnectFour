#ifndef UI_H
#define UI_H

#include "Game.h"  // Inclui definições do tabuleiro e GameMode

// Mostra o menu principal do jogo
void ui_show_main_menu(void);

// Pergunta o modo de jogo (Jogador vs Jogador ou Jogador vs IA)
GameMode ui_ask_game_mode(void);

// Pergunta quem será o primeiro jogador (X ou O)
char ui_ask_first_player(void);

// Mostra o tabuleiro na tela
void ui_display_board(const char board[BOARD_ROWS][BOARD_COLS]);

// Solicita a jogada de um jogador humano
int ui_get_human_move(const char board[BOARD_ROWS][BOARD_COLS], char current_piece);

// Mostra a mensagem de vitória
void ui_show_winner_message(char winner_piece, GameMode mode);

// Mostra a mensagem de empate
void ui_show_draw_message(void);

// Mostra a mensagem de fim de jogo
void ui_show_end_message(void);

// Pergunta se o jogador deseja jogar novamente
char ui_ask_play_again(void);

#endif




