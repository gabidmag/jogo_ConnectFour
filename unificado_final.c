/* Connect Four - Arquivo Unificado Completo */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

/* ========== DEFINIÇÕES UNIFICADAS ========== */
/* NOTA: Removi definições duplicadas e padronizei os nomes */

#define LINHAS 6
#define COLUNAS 7
#define PROFUNDIDADE_PADRAO 5

#define EMPTY_CELL '.'
#define PLAYER1_PIECE 'X'
#define PLAYER2_PIECE 'O'
#define PECA_IA 'O'
#define PECA_JOGADOR 'X'

/* NOTA: Usei apenas LINHAS/COLUNAS para evitar duplicação */
#define BOARD_ROWS LINHAS
#define BOARD_COLS COLUNAS
#define ROWS LINHAS
#define COLS COLUNAS

typedef enum {
    MODE_PVP,
    MODE_PVIA
} GameMode;

/* ========== PROTÓTIPOS DE FUNÇÕES ========== */
/* NOTA: Juntei todos os protótipos que estavam nos .h aqui */

/* Funções do Board */
void init_board(char board[ROWS][COLS]);
void print_board(char board[ROWS][COLS]);
int drop_piece_board(char board[ROWS][COLS], int col, char piece);
int check_full(char board[ROWS][COLS]);

/* Funções do Game */
void initialize_board(char board[BOARD_ROWS][BOARD_COLS]);
void display_board(char board[BOARD_ROWS][BOARD_COLS]);
int get_human_move(char board[BOARD_ROWS][BOARD_COLS], char current_piece);
int get_ia_move(char board[BOARD_ROWS][BOARD_COLS]);
int drop_piece(char board[BOARD_ROWS][BOARD_COLS], int col, char piece);
int check_win(char board[BOARD_ROWS][BOARD_COLS], char piece);
int check_draw(char board[BOARD_ROWS][BOARD_COLS]);
void run_game();

/* Funções da IA */
int IA_escolherColuna(char tabuleiro[LINHAS][COLUNAS]);
bool IA_existeJogadaValida(char tabuleiro[LINHAS][COLUNAS]);

/* Funções da UI */
void ui_show_main_menu(void);
GameMode ui_ask_game_mode(void);
char ui_ask_first_player(void);
void ui_display_board(const char board[BOARD_ROWS][BOARD_COLS]);
int ui_get_human_move(const char board[BOARD_ROWS][BOARD_COLS], char current_piece);
void ui_show_winner_message(char winner_piece, GameMode mode);
void ui_show_draw_message(void);
void ui_show_end_message(void);
char ui_ask_play_again(void);

/* Funções Utilitárias */
void clear_screen(void);
int rand_between(int min, int max);
void press_enter_to_continue(void);
void print_divider(char c, int length);

/* ========== IMPLEMENTAÇÃO DA IA ========== */
/* NOTA: Mantive toda a IA, mas removi o 'static' das funções auxiliares */

typedef struct {
    int coluna;
    int pontuacao;
} MovimentoIA;

bool haVitoria(char tabuleiro[LINHAS][COLUNAS], char peca) {
    /* Horizontal */
    for (int linha = 0; linha < LINHAS; linha++) {
        for (int coluna = 0; coluna <= COLUNAS - 4; coluna++) {
            if (tabuleiro[linha][coluna] == peca &&
                tabuleiro[linha][coluna + 1] == peca &&
                tabuleiro[linha][coluna + 2] == peca &&
                tabuleiro[linha][coluna + 3] == peca) {
                return true;
            }
        }
    }

    /* Vertical */
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        for (int linha = 0; linha <= LINHAS - 4; linha++) {
            if (tabuleiro[linha][coluna] == peca &&
                tabuleiro[linha + 1][coluna] == peca &&
                tabuleiro[linha + 2][coluna] == peca &&
                tabuleiro[linha + 3][coluna] == peca) {
                return true;
            }
        }
    }

    /* Diagonais \ */
    for (int linha = 0; linha <= LINHAS - 4; linha++) {
        for (int coluna = 0; coluna <= COLUNAS - 4; coluna++) {
            if (tabuleiro[linha][coluna] == peca &&
                tabuleiro[linha + 1][coluna + 1] == peca &&
                tabuleiro[linha + 2][coluna + 2] == peca &&
                tabuleiro[linha + 3][coluna + 3] == peca) {
                return true;
            }
        }
    }

    /* Diagonais / */
    for (int linha = 3; linha < LINHAS; linha++) {
        for (int coluna = 0; coluna <= COLUNAS - 4; coluna++) {
            if (tabuleiro[linha][coluna] == peca &&
                tabuleiro[linha - 1][coluna + 1] == peca &&
                tabuleiro[linha - 2][coluna + 2] == peca &&
                tabuleiro[linha - 3][coluna + 3] == peca) {
                return true;
            }
        }
    }

    return false;
}

bool tabuleiroCheio(char tabuleiro[LINHAS][COLUNAS]) {
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        if (tabuleiro[0][coluna] == EMPTY_CELL) {
            return false;
        }
    }
    return true;
}

int encontrarLinhaDisponivel(char tabuleiro[LINHAS][COLUNAS], int coluna) {
    for (int linha = LINHAS - 1; linha >= 0; linha--) {
        if (tabuleiro[linha][coluna] == EMPTY_CELL) {
            return linha;
        }
    }
    return -1;
}

int pontuarSequencia(int contIA, int contHumano, int contVazio) {
    if (contIA == 4) {
        return 100000;
    }
    if (contIA == 3 && contVazio == 1) {
        return 100;
    }
    if (contIA == 2 && contVazio == 2) {
        return 10;
    }

    if (contHumano == 4) {
        return -100000;
    }
    if (contHumano == 3 && contVazio == 1) {
        return -90;
    }
    if (contHumano == 2 && contVazio == 2) {
        return -12;
    }

    return 0;
}

int avaliarTabuleiro(char tabuleiro[LINHAS][COLUNAS]) {
    int pontuacao = 0;
    int colunaCentral = COLUNAS / 2;

    for (int linha = 0; linha < LINHAS; linha++) {
        if (tabuleiro[linha][colunaCentral] == PECA_IA) {
            pontuacao += 6;
        } else if (tabuleiro[linha][colunaCentral] == PECA_JOGADOR) {
            pontuacao -= 6;
        }
    }

    /* Janelas horizontais */
    for (int linha = 0; linha < LINHAS; linha++) {
        for (int coluna = 0; coluna <= COLUNAS - 4; coluna++) {
            int contIA = 0, contHumano = 0, contVazio = 0;
            for (int i = 0; i < 4; i++) {
                char casa = tabuleiro[linha][coluna + i];
                if (casa == PECA_IA) contIA++;
                else if (casa == PECA_JOGADOR) contHumano++;
                else contVazio++;
            }
            pontuacao += pontuarSequencia(contIA, contHumano, contVazio);
        }
    }

    /* Janelas verticais */
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        for (int linha = 0; linha <= LINHAS - 4; linha++) {
            int contIA = 0, contHumano = 0, contVazio = 0;
            for (int i = 0; i < 4; i++) {
                char casa = tabuleiro[linha + i][coluna];
                if (casa == PECA_IA) contIA++;
                else if (casa == PECA_JOGADOR) contHumano++;
                else contVazio++;
            }
            pontuacao += pontuarSequencia(contIA, contHumano, contVazio);
        }
    }

    /* Diagonais \ */
    for (int linha = 0; linha <= LINHAS - 4; linha++) {
        for (int coluna = 0; coluna <= COLUNAS - 4; coluna++) {
            int contIA = 0, contHumano = 0, contVazio = 0;
            for (int i = 0; i < 4; i++) {
                char casa = tabuleiro[linha + i][coluna + i];
                if (casa == PECA_IA) contIA++;
                else if (casa == PECA_JOGADOR) contHumano++;
                else contVazio++;
            }
            pontuacao += pontuarSequencia(contIA, contHumano, contVazio);
        }
    }

    /* Diagonais / */
    for (int linha = 3; linha < LINHAS; linha++) {
        for (int coluna = 0; coluna <= COLUNAS - 4; coluna++) {
            int contIA = 0, contHumano = 0, contVazio = 0;
            for (int i = 0; i < 4; i++) {
                char casa = tabuleiro[linha - i][coluna + i];
                if (casa == PECA_IA) contIA++;
                else if (casa == PECA_JOGADOR) contHumano++;
                else contVazio++;
            }
            pontuacao += pontuarSequencia(contIA, contHumano, contVazio);
        }
    }

    return pontuacao;
}

int minimax(char tabuleiro[LINHAS][COLUNAS], int profundidade, bool turnoIA, int alpha, int beta) {
    bool vitoriaIA = haVitoria(tabuleiro, PECA_IA);
    bool vitoriaJogador = haVitoria(tabuleiro, PECA_JOGADOR);

    if (vitoriaIA) {
        return 1000000 + profundidade;
    }
    if (vitoriaJogador) {
        return -1000000 - profundidade;
    }
    if (profundidade == 0 || tabuleiroCheio(tabuleiro)) {
        return avaliarTabuleiro(tabuleiro);
    }

    if (turnoIA) {
        int melhorValor = INT_MIN;
        for (int coluna = 0; coluna < COLUNAS; coluna++) {
            int linha = encontrarLinhaDisponivel(tabuleiro, coluna);
            if (linha == -1) {
                continue;
            }

            tabuleiro[linha][coluna] = PECA_IA;
            int valor = minimax(tabuleiro, profundidade - 1, false, alpha, beta);
            tabuleiro[linha][coluna] = EMPTY_CELL;

            if (valor > melhorValor) {
                melhorValor = valor;
            }
            if (valor > alpha) {
                alpha = valor;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return melhorValor;
    } else {
        int melhorValor = INT_MAX;
        for (int coluna = 0; coluna < COLUNAS; coluna++) {
            int linha = encontrarLinhaDisponivel(tabuleiro, coluna);
            if (linha == -1) {
                continue;
            }

            tabuleiro[linha][coluna] = PECA_JOGADOR;
            int valor = minimax(tabuleiro, profundidade - 1, true, alpha, beta);
            tabuleiro[linha][coluna] = EMPTY_CELL;

            if (valor < melhorValor) {
                melhorValor = valor;
            }
            if (valor < beta) {
                beta = valor;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return melhorValor;
    }
}

int IA_escolherColuna(char tabuleiro[LINHAS][COLUNAS]) {
    MovimentoIA melhorJogada = { .coluna = -1, .pontuacao = INT_MIN };

    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        int linha = encontrarLinhaDisponivel(tabuleiro, coluna);
        if (linha == -1) {
            continue;
        }

        tabuleiro[linha][coluna] = PECA_IA;
        int pontuacao = minimax(tabuleiro, PROFUNDIDADE_PADRAO - 1, false, INT_MIN, INT_MAX);
        tabuleiro[linha][coluna] = EMPTY_CELL;

        if (pontuacao > melhorJogada.pontuacao ||
            (pontuacao == melhorJogada.pontuacao && melhorJogada.coluna != -1 &&
             abs(coluna - COLUNAS / 2) < abs(melhorJogada.coluna - COLUNAS / 2))) {
            melhorJogada.coluna = coluna;
            melhorJogada.pontuacao = pontuacao;
        }
    }

    return melhorJogada.coluna;
}

bool IA_existeJogadaValida(char tabuleiro[LINHAS][COLUNAS]) {
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        if (tabuleiro[0][coluna] == EMPTY_CELL) {
            return true;
        }
    }
    return false;
}

/* ========== IMPLEMENTAÇÃO DO BOARD ========== */

void init_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = EMPTY_CELL;
}

void print_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++)
            printf(" %c |", board[i][j]);
        printf("\n");
    }
    printf("  1   2   3   4   5   6   7\n");
}

int drop_piece_board(char board[ROWS][COLS], int col, char piece) {
    if (col < 0 || col >= COLS) return 0;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == EMPTY_CELL) {
            board[i][col] = piece;
            return 1;
        }
    }
    return 0;
}

int check_full(char board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++)
        if (board[0][j] == EMPTY_CELL) return 0;
    return 1;
}

/* ========== IMPLEMENTAÇÃO DO GAME ========== */
/* NOTA: Corrigi conflitos de nomes e unifiquei a lógica */

int char_to_col_index(char col_char) {
    return toupper(col_char) - 'A';
}

int is_valid_col_index(int col_index) {
    return col_index >= 0 && col_index < BOARD_COLS;
}

int check_direction(char board[BOARD_ROWS][BOARD_COLS], int r, int c, int dr, int dc, char piece) {
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
    return ui_get_human_move(board, current_piece);
}

int get_ia_move(char board[BOARD_ROWS][BOARD_COLS]) {
    return IA_escolherColuna(board);
}

int check_win(char board[BOARD_ROWS][BOARD_COLS], char piece) {
    // Reutiliza a função haVitoria da IA que é mais completa
    return haVitoria(board, piece);
}

int check_draw(char board[BOARD_ROWS][BOARD_COLS]) {
    for (int j = 0; j < BOARD_COLS; j++) {
        if (board[BOARD_ROWS - 1][j] == EMPTY_CELL) {
            return 0;
        }
    }
    return 1;
}

/* ========== IMPLEMENTAÇÃO DO GAME (continuação) ========== */

void run_game() {
    char board[BOARD_ROWS][BOARD_COLS];
    char play_again_char;

    srand(time(NULL));

    do {
        initialize_board(board);
        char current_turn_piece;
        int game_over = 0;
        GameMode game_mode;

        game_mode = ui_ask_game_mode();
        current_turn_piece = ui_ask_first_player();

        while (!game_over) {
            ui_display_board(board);

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
                ui_display_board(board);
                ui_show_winner_message(winning_piece, game_mode);
                game_over = 1;
            }

            if (!game_over && check_draw(board)) {
                ui_display_board(board);
                ui_show_draw_message();
                game_over = 1;
            }

            if (!game_over) {
                current_turn_piece = (current_turn_piece == PLAYER1_PIECE) ? PLAYER2_PIECE : PLAYER1_PIECE;
            }
        }

        ui_show_end_message();
        play_again_char = ui_ask_play_again();

    } while (play_again_char == 'S');
}

/* ========== IMPLEMENTAÇÃO DA UI ========== */
/* NOTA: Adaptei as funções UI para trabalhar com o código unificado */

void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int letter_to_column_index(char column_letter) {
    return toupper((unsigned char)column_letter) - 'A';
}

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
        if (!is_valid_col_index(column_index)) {
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

void ui_show_winner_message(char winner_piece, GameMode mode) {
    if (winner_piece == PLAYER1_PIECE) {
        printf("Parabéns, Jogador X venceu!\n");
    } else if (winner_piece == PLAYER2_PIECE && mode == MODE_PVP) {
        printf("Parabéns, Jogador O venceu!\n");
    } else {
        printf("A IA venceu!\n");
    }
}

void ui_show_draw_message(void) {
    printf("Empate!\n");
}

void ui_show_end_message(void) {
    printf("Fim de jogo.\n");
}

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

/* ========== IMPLEMENTAÇÃO DAS UTILITÁRIAS ========== */
/* NOTA: Mantive apenas as funções realmente utilizadas */

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int rand_between(int min, int max) {
    if (max <= min) return min;
    return (rand() % (max - min + 1)) + min;
}

void press_enter_to_continue(void) {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n');
}

void print_divider(char c, int length) {
    for (int i = 0; i < length; i++) {
        putchar(c);
    }
    putchar('\n');
}

/* NOTA: Removi evaluate_board() pois não era usada no código */

/* ========== FUNÇÃO MAIN ========== */

void menu(void) {
    clear_screen();
    print_divider('=', 30);
    printf("   Jogo Connect Four (Lig-4)\n");
    print_divider('=', 30);
    printf("1. Jogar\n");
    printf("0. Sair\n");
    print_divider('-', 30);
    printf("Escolha uma opcao: ");
}

int main(void) {
    srand((unsigned) time(NULL));
    int opcao;

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1:
                clear_screen();
                run_game();             
                press_enter_to_continue();
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                press_enter_to_continue();
        }
    } while (opcao != 0);

    return 0;
}