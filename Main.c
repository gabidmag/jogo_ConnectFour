#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "Game.h"

// Exibe o menu principal do jogo
void menu(void) {
    clear_screen();                    // Limpa a tela
    print_divider('=', 30);           // Linha decorativa
    printf("   Jogo Connect Four (Lig-4)\n");
    print_divider('=', 30);
    printf("1. Jogar\n");             // Opção para iniciar jogo
    printf("0. Sair\n");              // Opção para sair
    print_divider('-', 30);
    printf("Escolha uma opcao: ");    // Solicita entrada do usuário
}

// Função principal do programa
int main(void) {
    srand((unsigned) time(NULL));     // Inicializa gerador de números aleatórios
    int opcao = -1;                   // Variável para armazenar escolha do usuário

    // Loop principal do programa
    do {
        menu();                       // Mostra o menu
        if (scanf("%d", &opcao) != 1) {  // Lê a opção do usuário
            // Entrada inválida (não é número)
            opcao = -1;
            while (getchar() != '\n'); // Limpa o buffer do teclado
        }

        // Processa a opção escolhida
        switch (opcao) {
            case 1:                   // Opção "Jogar"
                clear_screen();       // Limpa a tela
                run_game();           // Inicia o jogo principal
                press_enter_to_continue();  // Aguarda Enter para continuar
                break;

            case 0:                   // Opção "Sair"
                printf("Saindo...\n");
                break;

            default:                  // Opção inválida
                printf("Opcao invalida!\n");
                press_enter_to_continue();  // Aguarda Enter
                break;
        }
    } while (opcao != 0);             // Repete até usuário escolher "Sair"

    return 0;                         // Fim do programa
}