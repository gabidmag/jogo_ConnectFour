#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "Game.h"

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
    int opcao = -1;

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            // Entrada inválida
            opcao = -1;
            while (getchar() != '\n'); // Limpa o buffer
        }

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
                break;
        }
    } while (opcao != 0);

    return 0;
}
