#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PATRICIA_TREE
#define PATRICIA_TREE

#define PATRICIA_MAX_CHAVE_TAMANHO 512


typedef struct Node {
  struct Node** filhos;
  char* prefixo;
  int contadorFilho;
  int nivel;
  int id;
  int ehNoFolha;
  int valor;
} Node;

typedef struct PatriciaTree {
  Node* raiz;
  int contador_no;
  int contador_chaves;
} PatriciaTree;


//Inicializar
PatriciaTree* init_patricia_tree();

// Encontrar
const int* encontrar_arvore(PatriciaTree* arvore, const char* chave);
Node* encontrar(Node*, const char*);

//Inserir
void inserir_arvore(PatriciaTree* arvore, const char* chave, int valor);
void inserir(Node*, const char*, int);
Node* novo_no(const char *, int, int);
void add_filho(Node*, const char*, int, int);
void anexar_filho(Node*, Node*);

//Remover
void remover_no_arvore(PatriciaTree* arvore, const char* chave);
void remover_no(PatriciaTree* arvore, Node *pai, Node *filho);
Node* encontrar_pai(Node *raiz, Node *filho);
Node* encontrar_no(Node *n, const char *chave);

//Auxiliares
int temchave_arvore(PatriciaTree* arvore, const char* chave) ;
int contador_chave(PatriciaTree* arvore);
void strmtc(const char*, const char*, char*);
int  strcnt(const char*, const char*);
void update_nivel(Node*, int);
void update_prefixo(Node*, const char*);

//Liberar
void free_no(Node*);
void free_arvore(PatriciaTree* arvore);

//Printar
void pPrint_arvore(PatriciaTree* arvore);
void print_no(Node*);
void print_arvore(Node*);

#endif 

