#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Funções públicas:
 *     int IA_escolherColuna(char tabuleiro[LINHAS][COLUNAS]);
 *         Retorna o índice da coluna (0 a 6) onde a IA deseja jogar.
 *         Retorna -1 se não existir jogada válida.
 *
 *     bool IA_existeJogadaValida(char tabuleiro[LINHAS][COLUNAS]);
 *         Útil para o main verificar rapidamente se a IA ainda pode jogar.
 *
 * A lógica adotada usa MiniMax com poda alpha-beta e uma heurística simples
 * para avaliar o tabuleiro. Comentários em português explicam cada parte.
 */

#define LINHAS 6
#define COLUNAS 7
#define PROFUNDIDADE_PADRAO 5

#define PECA_IA 'O'
#define PECA_JOGADOR 'X'
#define CASA_VAZIA '.'

/* Estrutura auxiliar para carregar a melhor jogada durante o MiniMax. */
typedef struct {
    int coluna;
    int pontuacao;
} MovimentoIA;

/* Assinaturas internas (estáticas) para manter o arquivo organizado. */
static bool haVitoria(char tabuleiro[LINHAS][COLUNAS], char peca);
static bool tabuleiroCheio(char tabuleiro[LINHAS][COLUNAS]);
static int encontrarLinhaDisponivel(char tabuleiro[LINHAS][COLUNAS], int coluna);
static int avaliarTabuleiro(char tabuleiro[LINHAS][COLUNAS]);
static int pontuarSequencia(int contIA, int contHumano, int contVazio);
static int minimax(char tabuleiro[LINHAS][COLUNAS], int profundidade, bool turnoIA, int alpha, int beta);

/*
 * Função pública principal: escolhe a coluna ideal para a jogada da IA.
 * A chamada é barata (só precisa passar o tabuleiro atual).
 */
int IA_escolherColuna(char tabuleiro[LINHAS][COLUNAS]) {
    MovimentoIA melhorJogada = { .coluna = -1, .pontuacao = INT_MIN };

    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        int linha = encontrarLinhaDisponivel(tabuleiro, coluna);
        if (linha == -1) {
            continue; /* coluna cheia */
        }

        tabuleiro[linha][coluna] = PECA_IA;
        int pontuacao = minimax(tabuleiro, PROFUNDIDADE_PADRAO - 1, false, INT_MIN, INT_MAX);
        tabuleiro[linha][coluna] = CASA_VAZIA;

        /* Preferimos melhores pontuações e, em empate, colunas centrais. */
        if (pontuacao > melhorJogada.pontuacao ||
            (pontuacao == melhorJogada.pontuacao && melhorJogada.coluna != -1 &&
             abs(coluna - COLUNAS / 2) < abs(melhorJogada.coluna - COLUNAS / 2))) {
            melhorJogada.coluna = coluna;
            melhorJogada.pontuacao = pontuacao;
        }
    }

    return melhorJogada.coluna;
}

/*
 * Permite ao código principal saber rapidamente se ainda existe jogada livre.
 */
bool IA_existeJogadaValida(char tabuleiro[LINHAS][COLUNAS]) {
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        if (tabuleiro[0][coluna] == CASA_VAZIA) {
            return true;
        }
    }
    return false;
}

/* ------------------------- Funções auxiliares -------------------------- */

/*
 * Checa se a peça recebida venceu (4 em linha). Procuramos em todas direções.
 */
static bool haVitoria(char tabuleiro[LINHAS][COLUNAS], char peca) {
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

/*
 * Determina se todo o tabuleiro está sem espaços livres.
 */
static bool tabuleiroCheio(char tabuleiro[LINHAS][COLUNAS]) {
    for (int coluna = 0; coluna < COLUNAS; coluna++) {
        if (tabuleiro[0][coluna] == CASA_VAZIA) {
            return false;
        }
    }
    return true;
}

/*
 * Retorna a linha disponível mais baixa em uma coluna, ou -1 se estiver cheia.
 */
static int encontrarLinhaDisponivel(char tabuleiro[LINHAS][COLUNAS], int coluna) {
    for (int linha = LINHAS - 1; linha >= 0; linha--) {
        if (tabuleiro[linha][coluna] == CASA_VAZIA) {
            return linha;
        }
    }
    return -1;
}

/*
 * Avalia o tabuleiro para determinar se a posição favorece a IA.
 * Valores positivos ajudam a IA, negativos ajudam o jogador humano.
 */
static int avaliarTabuleiro(char tabuleiro[LINHAS][COLUNAS]) {
    int pontuacao = 0;

    /* Bônus por ocupar a coluna central: normalmente oferece mais vitórias. */
    int colunaCentral = COLUNAS / 2;
    for (int linha = 0; linha < LINHAS; linha++) {
        if (tabuleiro[linha][colunaCentral] == PECA_IA) {
            pontuacao += 6;
        } else if (tabuleiro[linha][colunaCentral] == PECA_JOGADOR) {
            pontuacao -= 6;
        }
    }

    /* Janelas horizontais. */
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

    /* Janelas verticais. */
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

/*
 * Define pesos para diferentes combinações em janelas de 4 posições.
 */
static int pontuarSequencia(int contIA, int contHumano, int contVazio) {
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

/*
 * MiniMax clássico com poda alpha-beta. O parâmetro turnoIA indica se o nível
 * atual está maximizando (true) ou minimizando (false).
 */
static int minimax(char tabuleiro[LINHAS][COLUNAS], int profundidade, bool turnoIA, int alpha, int beta) {
    bool vitoriaIA = haVitoria(tabuleiro, PECA_IA);
    bool vitoriaJogador = haVitoria(tabuleiro, PECA_JOGADOR);

    if (vitoriaIA) {
        /* Profundidade adicionada para incentivar vitórias rápidas. */
        return 1000000 + profundidade;
    }
    if (vitoriaJogador) {
        /* Penalidade maior para derrotas, descontando profundidade. */
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
            tabuleiro[linha][coluna] = CASA_VAZIA;

            if (valor > melhorValor) {
                melhorValor = valor;
            }
            if (valor > alpha) {
                alpha = valor;
            }
            if (beta <= alpha) {
                break; /* poda */
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
            tabuleiro[linha][coluna] = CASA_VAZIA;

            if (valor < melhorValor) {
                melhorValor = valor;
            }
            if (valor < beta) {
                beta = valor;
            }
            if (beta <= alpha) {
                break; /* poda */
            }
        }
        return melhorValor;
    }
}
