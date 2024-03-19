#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pat.h"

#define FILENAME "./palavras.txt"


int main() {
    //Inicializa a arvore
    PatriciaTree* arvore = init_patricia_tree();

    char chave[PATRICIA_MAX_CHAVE_TAMANHO];
    int valor = 0;
    //Abre arquivo com as chaves e valores
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }


    printf("Adicionando palavras e valores do arquivo...\n");

    while (fscanf(file, "%s %d", chave, &valor) == 2) {
        inserir_arvore(arvore, chave, valor);
    }

    fclose(file);


    pPrint_arvore(arvore);

    // Menu interativo
    char opcao;
    do {
        printf("\nMenu:\n");
        printf("  (I)nserir\n");
        printf("  (R)emover\n");
        printf("  (E)ncontrar\n");
        printf("  (@) Sair\n");
        printf("  Digite sua opcao: ");
        scanf(" %c", &opcao);
        switch (opcao) {
            case 'I':
            case 'i':
                printf("Digite chave para inserir: ");
                scanf("%s", chave);
                printf("Digite o valor: ");
                scanf("%d", &valor);
                inserir_arvore(arvore, chave, valor);
                pPrint_arvore(arvore);
                printf("\n----------------------------------------\n");
                break;
            case 'R':
            case 'r':
                printf("Digite chave para remover: ");
                scanf("%s", chave);
                remover_no_arvore(arvore, chave);
                pPrint_arvore(arvore);
                printf("\n----------------------------------------\n");
                break;
            case 'E':
            case 'e':
                printf("Digite chave para encontrar: ");
                scanf("%s", chave);
                const int* val_ptr = encontrar_arvore(arvore, chave);
                if (val_ptr) {
                    printf("Chave \"%s\" encontrada com valor: %d\n", chave, *val_ptr);
                } else {
                    printf("Chave \"%s\" nao encontrada.\n", chave);
                }
                printf("\n----------------------------------------\n");
                break;
            case '@':
                printf("SAINDO...\n");
                break;
            default:
                printf("Opção invalida. Tente novamente.\n");
        }
    } while (opcao != '@');

    free_arvore(arvore);
    return 0;
}
