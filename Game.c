#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "game.h"

// Converte letra da coluna (A-G) para índice numérico (0-6)
static int char_to_col_index(char col_char) {
    return toupper(col_char) - 'A';  // A=0, B=1, C=2, etc.
}

// Verifica se índice da coluna é válido
static int is_valid_col_index(int col_index) {
    return col_index >= 0 && col_index < BOARD_COLS;
}

// Verifica 4 peças em linha em uma direção específica
static int check_direction(char board[BOARD_ROWS][BOARD_COLS], int r, int c, int dr, int dc, char piece) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int nr = r + i * dr;  // Nova linha
        int nc = c + i * dc;  // Nova coluna
        // Verifica se está dentro do tabuleiro e tem a peça
        if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS && board[nr][nc] == piece) {
            count++;
        } else {
            break;
        }
    }
    return count == 4;  // Retorna 1 se encontrou 4 em linha
}

// Inicializa tabuleiro com células vazias
void initialize_board(char board[BOARD_ROWS][BOARD_COLS]) {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            board[i][j] = EMPTY_CELL;  // Preenche com pontos
        }
    }
}

// Exibe tabuleiro com coordenadas (A-G e 1-6)
void display_board(char board[BOARD_ROWS][BOARD_COLS]) {
    printf("\n ");
    for (int j = 0; j < BOARD_COLS; j++) {
        printf(" %c", 'A' + j);  // Letras das colunas: A B C D E F G
    }
    printf("\n");

    // Mostra linhas de BAIXO para CIMA (6 até 1)
    for (int i = BOARD_ROWS - 1; i >= 0; i--) {
        printf("%d", i + 1);  // Números das linhas
        for (int j = 0; j < BOARD_COLS; j++) {
            printf(" %c", board[i][j]);  // Conteúdo de cada célula
        }
        printf("\n");
    }
    printf("\n");
}

// Coloca peça na coluna (versão diferente da board.c)
int drop_piece(char board[BOARD_ROWS][BOARD_COLS], int col, char piece) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        if (board[r][col] == EMPTY_CELL) {  // Encontrou espaço vazio
            board[r][col] = piece;          // Coloca a peça
            return r;                       // Retorna a linha onde caiu
        }
    }
    return -1;  // Coluna cheia
}

// Obtém jogada do jogador humano com validação
int get_human_move(char board[BOARD_ROWS][BOARD_COLS], char current_piece) {
    char col_char;
    int col_index = -1;
    int valid_input = 0;

    while (!valid_input) {
        printf("Jogador %c, escolha sua jogada (A-G): ", current_piece);
        if (scanf(" %c", &col_char) != 1) {  // Lê a jogada
            printf("Entrada inválida. Por favor, digite uma letra entre A e G.\n");
            while (getchar() != '\n');  // Limpa buffer
            continue;
        }

        col_index = char_to_col_index(col_char);  // Converte para número

        if (!is_valid_col_index(col_index)) {  // Verifica se coluna existe
            printf("Por favor, responda com: A, B, C, D, E, F ou G.\n");
            continue;
        }

        if (board[BOARD_ROWS - 1][col_index] != EMPTY_CELL) {  // Verifica se coluna está cheia
            printf("Coluna cheia! Escolha outra coluna.\n");
            continue;
        }
        valid_input = 1;  // Jogada válida
    }
    return col_index;
}

// Gera jogada aleatória para a IA
int get_ia_move(char board[BOARD_ROWS][BOARD_COLS]) {
    int col_index;
    do {
        col_index = rand() % BOARD_COLS;  // Escolhe coluna aleatória
    } while (board[BOARD_ROWS - 1][col_index] != EMPTY_CELL);  // Até achar coluna não cheia

    printf("IA jogou na coluna %c.\n", 'A' + col_index);
    return col_index;
}

// Verifica se há um vencedor em qualquer direção
int check_win(char board[BOARD_ROWS][BOARD_COLS], char piece) {
    // Verifica HORIZONTAL (→)
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c <= BOARD_COLS - 4; c++) {
            if (check_direction(board, r, c, 0, 1, piece)) return 1;
        }
    }

    // Verifica VERTICAL (↓)
    for (int r = 0; r <= BOARD_ROWS - 4; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (check_direction(board, r, c, 1, 0, piece)) return 1;
        }
    }

    // Verifica DIAGONAL (↘)
    for (int r = 0; r <= BOARD_ROWS - 4; r++) {
        for (int c = 0; c <= BOARD_COLS - 4; c++) {
            if (check_direction(board, r, c, 1, 1, piece)) return 1;
        }
    }

    // Verifica DIAGONAL (↗)
    for (int r = 3; r < BOARD_ROWS; r++) {
        for (int c = 0; c <= BOARD_COLS - 4; c++) {
            if (check_direction(board, r, c, -1, 1, piece)) return 1;
        }
    }

    return 0;  // Ninguém venceu
}

// Verifica se o jogo terminou em empate
int check_draw(char board[BOARD_ROWS][BOARD_COLS]) {
    for (int j = 0; j < BOARD_COLS; j++) {
        if (board[BOARD_ROWS - 1][j] == EMPTY_CELL) {
            return 0;  // Ainda tem coluna com espaço
        }
    }
    return 1;  // Todas as colunas estão cheias - empate
}

// FUNÇÃO PRINCIPAL DO JOGO - Controla todo o fluxo
void run_game() {
    char board[BOARD_ROWS][BOARD_COLS];  // Tabuleiro do jogo
    char play_again_char;

    srand(time(NULL));  // Inicializa gerador de números aleatórios

    do {
        initialize_board(board);  // Prepara novo tabuleiro
        char current_turn_piece;
        int game_over = 0;
        char first_player_choice_char;
        GameMode game_mode;

        // Seleção do modo de jogo
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

        // Escolha de quem começa
        while (1) {
            printf("Deseja que o Jogador X jogue primeiro? (S/N): ");
            if (scanf(" %c", &first_player_choice_char) != 1) {
                printf("Entrada inválida. Por favor, responda com 'S' ou 'N'.\n");
                while (getchar() != '\n');
                continue;
            }
            first_player_choice_char = toupper(first_player_choice_char);

            if (first_player_choice_char == 'S') {
                current_turn_piece = PLAYER1_PIECE;  // X começa
                break;
            } else if (first_player_choice_char == 'N') {
                current_turn_piece = PLAYER2_PIECE;  // O começa
                break;
            } else {
                printf("Por favor, responder com 'S' ou com 'N'.\n");
            }
            while (getchar() != '\n');
        }
        while (getchar() != '\n');

        // LOOP PRINCIPAL DO JOGO
        while (!game_over) {
            display_board(board);  // Mostra tabuleiro

            int col = -1;
            char winning_piece = EMPTY_CELL;

            // Turno do jogador atual
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
                    col = get_ia_move(board);  // Jogada da IA
                    drop_piece(board, col, PLAYER2_PIECE);
                    winning_piece = PLAYER2_PIECE;
                }
            }

            // Verifica vitória
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

            // Verifica empate
            if (!game_over && check_draw(board)) {
                display_board(board);
                printf("Empate!\n");
                game_over = 1;
            }

            // Alterna turno se jogo não acabou
            if (!game_over) {
                current_turn_piece = (current_turn_piece == PLAYER1_PIECE) ? PLAYER2_PIECE : PLAYER1_PIECE;
            }
        }

        // Pergunta se quer jogar novamente
        printf("Deseja jogar novamente? (S/N): ");
        if (scanf(" %c", &play_again_char) != 1) {
            play_again_char = 'N';
        }
        while (getchar() != '\n');
        play_again_char = toupper(play_again_char);

    } while (play_again_char == 'S');  // Repete se quiser jogar novamente
}