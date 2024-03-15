#ifndef PATRICIA_TREE
#define PATRICIA_TREE

#define PATRICIA_MAX_CHAVE_TAMANHO 512

typedef struct PatriciaTree {
  void (*inserir)(const char*, int);
  const int* (*encontrar)(const char*);
  void (*remover)(const char*);
  void (*print)(void);
  void (*free)(void);
  int (*temchave)(const char*);
  int (*contadorchave)(void);
} PatriciaTree;


typedef struct Node {
  struct Node** filhos;
  char* prefixo;
  char *palavra;
  int contadorFilho;
  int nivel;
  int id;
  int ehNoFolha;
  int valor;
} Node;

//Inicializar
PatriciaTree* init_patricia_tree();

// Encontrar
const int* encontrar_arvore(const char*);
Node* _encontrar(Node*, const char*);

//Inserir
void inserir_arvore(const char*, int);
void _inserir(Node*, const char*, int);
Node* _novo_no(const char *, int, int);
void _add_filho(Node*, const char*, int, int);
void _anexar_filho(Node*, Node*);

//Remover
void remover_no_arvore(const char* chave);
Node* _encontrar_pai(Node *raiz, Node *filho);
Node* _encontrar_no(Node *n, const char *chave);

//Auxiliares
int temchave_arvore();
int contador_chave();
void _strmtc(const char*, const char*, char*);
int  _strcnt(const char*, const char*);
void _update_nivel(Node*, int);
void _update_prefixo(Node*, const char*);

//Liberar
void _free_node(Node*);
void rtree_free();

//Printar
void rtree_print();
void _print_no(Node*);
void _print_arvore(Node*);





#endif 