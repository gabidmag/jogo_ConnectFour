#include <ctype.h>
#include <stdio.h>

#include "Game.h"
#include "ui.h"
#include "utils.h"

// Limpa o buffer de entrada
static void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

// Converte a letra da coluna para um índice numérico
static int letter_to_column_index(char column_letter) {
    return toupper((unsigned char)column_letter) - 'A';
}

// Verifica se o índice da coluna é válido
static int is_valid_column_index(int column_index) {
    return column_index >= 0 && column_index < BOARD_COLS;
}

// Mostra o menu principal
void ui_show_main_menu(void) {
    clear_screen();
    print_divider('=', 30);
    printf("   Jogo Lig-4 (Connect Four)\n");
    print_divider('=', 30);
    printf("1. Jogar\n");
    printf("0. Sair\n");
    print_divider('-', 30);
    printf("Escolha uma opção: ");
}

// Pergunta o modo de jogo
GameMode ui_ask_game_mode(void) {
    char mode_choice;
    for (;;) {
        printf("Escolha o modo de jogo:\n");
        printf("1 - Jogador vs Jogador\n");
        printf("2 - Jogador vs IA\n");
        printf("Sua escolha: ");
        if (scanf(" %c", &mode_choice) != 1) {
            printf("Entrada inválida. Por favor, digite '1' ou '2'.\n");
            clear_input();
            continue;
        }
        clear_input();
        if (mode_choice == '1') return MODE_PVP;
        if (mode_choice == '2') return MODE_PVIA;
        printf("Opção inválida. Por favor, digite '1' ou '2'.\n");
    }
}

// Pergunta quem joga primeiro
char ui_ask_first_player(void) {
    char first_choice;
    for (;;) {
        printf("Deseja que o Jogador X jogue primeiro? (S/N): ");
        if (scanf(" %c", &first_choice) != 1) {
            printf("Entrada inválida. Por favor, responda com 'S' ou 'N'.\n");
            clear_input();
            continue;
        }
        clear_input();
        first_choice = toupper((unsigned char)first_choice);
        if (first_choice == 'S') return PLAYER1_PIECE;
        if (first_choice == 'N') return PLAYER2_PIECE;
        printf("Por favor, responda com 'S' ou 'N'.\n");
    }
}

// Exibe o tabuleiro na tela
void ui_display_board(const char board[BOARD_ROWS][BOARD_COLS]) {
    printf("\n ");
    for (int j = 0; j < BOARD_COLS; j++) printf(" %c", 'A' + j);
    printf("\n");

    for (int i = BOARD_ROWS - 1; i >= 0; i--) {
        printf("%d", i + 1);
        for (int j = 0; j < BOARD_COLS; j++) printf(" %c", board[i][j]);
        printf("\n");
    }
    printf("\n");
}

// Lê a jogada de um jogador humano
int ui_get_human_move(const char board[BOARD_ROWS][BOARD_COLS], char current_piece) {
    char column_letter;
    int column_index;
    for (;;) {
        printf("Jogador %c, escolha sua jogada (A-%c): ",
               current_piece, 'A' + BOARD_COLS - 1);
        if (scanf(" %c", &column_letter) != 1) {
            printf("Entrada inválida. Por favor, digite uma letra válida.\n");
            clear_input();
            continue;
        }
        clear_input();
        column_index = letter_to_column_index(column_letter);
        if (!is_valid_column_index(column_index)) {
            printf("Por favor, responda com letras entre A e %c.\n", 'A' + BOARD_COLS - 1);
            continue;
        }
        if (board[BOARD_ROWS - 1][column_index] != EMPTY_CELL) {
            printf("Coluna cheia! Escolha outra coluna.\n");
            continue;
        }
        return column_index;
    }
}

// Mostra mensagem de vitória
void ui_show_winner_message(char winner_piece, GameMode mode) {
    if (winner_piece == PLAYER1_PIECE) {
        printf("Parabéns, Jogador X venceu!\n");
    } else if (winner_piece == PLAYER2_PIECE && mode == MODE_PVP) {
        printf("Parabéns, Jogador O venceu!\n");
    } else {
        printf("A IA venceu!\n");
    }
}

// Mostra mensagem de empate
void ui_show_draw_message(void) {
    printf("Empate!\n");
}

// Mostra mensagem de fim de jogo
void ui_show_end_message(void) {
    printf("Fim de jogo.\n");
}

// Pergunta se o jogador quer jogar novamente
char ui_ask_play_again(void) {
    char play_again;
    for (;;) {
        printf("Deseja jogar novamente? (S/N): ");
        if (scanf(" %c", &play_again) != 1) {
            clear_input();
            printf("Entrada inválida. Por favor, responda com 'S' ou 'N'.\n");
            continue;
        }
        clear_input();
        play_again = (char)toupper((unsigned char)play_again);
        if (play_again == 'S' || play_again == 'N') {
            return play_again;
        }
        printf("Por favor, responda com 'S' ou 'N'.\n");
    }
}