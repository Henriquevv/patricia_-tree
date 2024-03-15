#include <stdio.h>
#include <stdlib.h>

#include "../include/patricia_tree.h"

#define FILENAME "include/palavras.txt"


int main() {
    //Inicializa a arvore
    PatriciaTree* tree = init_patricia_tree();

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
        tree->inserir(chave, valor);
    }

    fclose(file);

    
    tree->print();

    // Menu interativo
    char opcao;
    do {
        printf("\nMenu:\n");
        printf("  (I)nserir\n");
        printf("  (R)emover\n");
        printf("  (E)ncontrar\n");
        printf("  (@) Sair\n");
        printf("  Digite sua opção: ");
        scanf(" %c", &opcao);
        switch (opcao) {
            case 'I':
            case 'i':
                printf("Digite chave para inserir: ");
                scanf("%s", chave);
                printf("Digite o valor: ");
                scanf("%d", &valor);
                tree->inserir(chave, valor);
                tree->print();
                printf("\n----------------------------------------\n");
                break;
            case 'R':
            case 'r':
                printf("Digite chave para remover: ");
                scanf("%s", chave);
                remover_no_arvore(chave);
                tree->print();
                printf("\n----------------------------------------\n");
                break;
            case 'E':
            case 'e':
                printf("Digite chave para encontrar: ");
                scanf("%s", chave);
                const int* val_ptr = tree->encontrar(chave);
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

    tree->free();
    return 0;
}
