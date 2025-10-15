#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "game.h"

static int char_to_col_index(char col_char) {
    return toupper(col_char) - 'A';
}

static int is_valid_col_index(int col_index) {
    return col_index >= 0 && col_index < BOARD_COLS;
}

static int check_direction(char board[BOARD_ROWS][BOARD_COLS], int r, int c, int dr, int dc, char piece) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int nr = r + i * dr;
        int nc = c + i * dc;
        if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS && board[nr][nc] == piece) {
            count++;
        } else {
            break;
        }
    }
    return count == 4;
}

void initialize_board(char board[BOARD_ROWS][BOARD_COLS]) {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            board[i][j] = EMPTY_CELL;
        }
    }
}

void display_board(char board[BOARD_ROWS][BOARD_COLS]) {
    printf("\n ");
    for (int j = 0; j < BOARD_COLS; j++) {
        printf(" %c", 'A' + j);
    }
    printf("\n");

    for (int i = BOARD_ROWS - 1; i >= 0; i--) {
        printf("%d", i + 1);
        for (int j = 0; j < BOARD_COLS; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int drop_piece(char board[BOARD_ROWS][BOARD_COLS], int col, char piece) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        if (board[r][col] == EMPTY_CELL) {
            board[r][col] = piece;
            return r;
        }
    }
    return -1;
}

int get_human_move(char board[BOARD_ROWS][BOARD_COLS], char current_piece) {
    char col_char;
    int col_index = -1;
    int valid_input = 0;

    while (!valid_input) {
        printf("Jogador %c, escolha sua jogada (A-G): ", current_piece);
        if (scanf(" %c", &col_char) != 1) {
            printf("Entrada inválida. Por favor, digite uma letra entre A e G.\n");
            while (getchar() != '\n');
            continue;
        }

        col_index = char_to_col_index(col_char);

        if (!is_valid_col_index(col_index)) {
            printf("Por favor, responda com: A, B, C, D, E, F ou G.\n");
            continue;
        }

        if (board[BOARD_ROWS - 1][col_index] != EMPTY_CELL) {
            printf("Coluna cheia! Escolha outra coluna.\n");
            continue;
        }
        valid_input = 1;
    }
    return col_index;
}

int get_ia_move(char board[BOARD_ROWS][BOARD_COLS]) {
    int col_index;
    do {
        col_index = rand() % BOARD_COLS;
    } while (board[BOARD_ROWS - 1][col_index] != EMPTY_CELL);

    printf("IA jogou na coluna %c.\n", 'A' + col_index);
    return col_index;
}

int check_win(char board[BOARD_ROWS][BOARD_COLS], char piece) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c <= BOARD_COLS - 4; c++) {
            if (check_direction(board, r, c, 0, 1, piece)) return 1;
        }
    }

    for (int r = 0; r <= BOARD_ROWS - 4; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (check_direction(board, r, c, 1, 0, piece)) return 1;
        }
    }

    for (int r = 0; r <= BOARD_ROWS - 4; r++) {
        for (int c = 0; c <= BOARD_COLS - 4; c++) {
            if (check_direction(board, r, c, 1, 1, piece)) return 1;
        }
    }

    for (int r = 3; r < BOARD_ROWS; r++) {
        for (int c = 0; c <= BOARD_COLS - 4; c++) {
            if (check_direction(board, r, c, -1, 1, piece)) return 1;
        }
    }

    return 0;
}

int check_draw(char board[BOARD_ROWS][BOARD_COLS]) {
    for (int j = 0; j < BOARD_COLS; j++) {
        if (board[BOARD_ROWS - 1][j] == EMPTY_CELL) {
            return 0;
        }
    }
    return 1;
}

void run_game() {
    char board[BOARD_ROWS][BOARD_COLS];
    char play_again_char;

    srand(time(NULL));

    do {
        initialize_board(board);
        char current_turn_piece;
        int game_over = 0;
        char first_player_choice_char;
        GameMode game_mode;

        char mode_choice_char;
        while (1) {
            printf("Escolha o modo de jogo:\n");
            printf("1 - Jogador vs Jogador\n");
            printf("2 - Jogador vs IA\n");
            printf("Sua escolha: ");
            if (scanf(" %c", &mode_choice_char) != 1) {
                printf("Entrada inválida. Por favor, digite '1' ou '2'.\n");
                while (getchar() != '\n');
                continue;
            }
            if (mode_choice_char == '1') {
                game_mode = MODE_PVP;
                break;
            } else if (mode_choice_char == '2') {
                game_mode = MODE_PVIA;
                break;
            } else {
                printf("Opção inválida. Por favor, digite '1' ou '2'.\n");
            }
            while (getchar() != '\n');
        }
        while (getchar() != '\n');

        while (1) {
            printf("Deseja que o Jogador X jogue primeiro? (S/N): ");
            if (scanf(" %c", &first_player_choice_char) != 1) {
                printf("Entrada inválida. Por favor, responda com 'S' ou 'N'.\n");
                while (getchar() != '\n');
                continue;
            }
            first_player_choice_char = toupper(first_player_choice_char);

            if (first_player_choice_char == 'S') {
                current_turn_piece = PLAYER1_PIECE;
                break;
            } else if (first_player_choice_char == 'N') {
                current_turn_piece = PLAYER2_PIECE;
                break;
            } else {
                printf("Por favor, responder com 'S' ou com 'N'.\n");
            }
            while (getchar() != '\n');
        }
        while (getchar() != '\n');

        while (!game_over) {
            display_board(board);

            int col = -1;
            char winning_piece = EMPTY_CELL;

            if (current_turn_piece == PLAYER1_PIECE) {
                col = get_human_move(board, PLAYER1_PIECE);
                drop_piece(board, col, PLAYER1_PIECE);
                winning_piece = PLAYER1_PIECE;
            } else {
                if (game_mode == MODE_PVP) {
                    col = get_human_move(board, PLAYER2_PIECE);
                    drop_piece(board, col, PLAYER2_PIECE);
                    winning_piece = PLAYER2_PIECE;
                } else {
                    col = get_ia_move(board);
                    drop_piece(board, col, PLAYER2_PIECE);
                    winning_piece = PLAYER2_PIECE;
                }
            }

            if (check_win(board, winning_piece)) {
                display_board(board);
                if (winning_piece == PLAYER1_PIECE) {
                    printf("Parabéns, Jogador X venceu!\n");
                } else if (winning_piece == PLAYER2_PIECE && game_mode == MODE_PVP) {
                    printf("Parabéns, Jogador O venceu!\n");
                } else {
                    printf("A IA venceu!\n");
                }
                game_over = 1;
            }

            if (!game_over && check_draw(board)) {
                display_board(board);
                printf("Empate!\n");
                game_over = 1;
            }

            if (!game_over) {
                current_turn_piece = (current_turn_piece == PLAYER1_PIECE) ? PLAYER2_PIECE : PLAYER1_PIECE;
            }
        }

        printf("Fim de jogo.\n");
        printf("Deseja jogar novamente? (S/N): ");
        if (scanf(" %c", &play_again_char) != 1) {
            play_again_char = 'N';
        }
        while (getchar() != '\n');
        play_again_char = toupper(play_again_char);

    } while (play_again_char == 'S');

    printf("
