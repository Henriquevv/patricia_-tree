#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pat.h"

int contador_no = 0;
int contador_chaves = 0;

PatriciaTree* arvore = NULL;

// INICIALIZAR A ARVORE
PatriciaTree* init_patricia_tree() {
  PatriciaTree* arvore = (PatriciaTree*)calloc(1, sizeof(PatriciaTree));
  if(arvore == NULL) {
    return NULL;
  }
  arvore->raiz = novo_no(NULL, 0, 0);
  arvore->contador_no = 0;
  arvore->contador_chaves = 0;
  return arvore;
}

// ----------------------------------------------FUNÇÃO INSERIR------------------------------------------------------ //
//Insere uma palavra na arvore
void inserir_arvore(PatriciaTree* arvore, const char* chave, int valor) {
  inserir(arvore->raiz, chave, valor);
  arvore->contador_chaves++;
}


void inserir(Node *n, const char *chave, int valor) {
  //Verifique se é uma chave vazia
  if (strlen(chave) == 0) {
    printf("OBS: TENTANDO INSERIR UMA CHAVE VAZIA.\n");
    return;
  }

  // Procura um filho relevante (prefixo)
  Node *rel = NULL;
  int relId = 0;
  for (int i = 0; i < n->contadorFilho; i++) {
    if (n->filhos[i]->prefixo[0] == chave[0]) {
      rel = n->filhos[i];
      relId = i;
      break;
    }
  }
  if (rel) {

    // Se houver, verifica se a chave é igual e atualiza seu valor
    if (strcmp(rel->prefixo, chave) == 0) {
      rel->ehNoFolha = 1;
      rel->valor = valor;
      return;
    }


    //Caso nao seja igual, continua
    //Verifica tamanho
    const int tamanhochave = strlen(chave);
    if (tamanhochave > PATRICIA_MAX_CHAVE_TAMANHO) {
      printf("FATAL: TAMANHO MAXIMO (%d bytes) EXCEDIDO.\n", tamanhochave);
    } 
    //Verifica se a chave contem o prefixo
    char *match = calloc(tamanhochave + 1, sizeof(char));
    strmtc(rel->prefixo, chave, match);
    if (strlen(match) == strlen(rel->prefixo)) {
      free((void *)match);
      inserir(rel, (chave + strlen(rel->prefixo)), valor);
      return;
    }

    // Verifica se o prefixo é uma chave completa ou parte de uma
    if (strlen(match) > 0) {
      Node *paiComum = novo_no(match, 0, 0);
      paiComum->nivel = n->nivel + 1;
      if(strlen(match) == strlen(chave)) {
        paiComum->ehNoFolha = 1;
        paiComum->valor = valor;
      } else {
        add_filho(paiComum, (chave + strlen(match)), 1, valor);
      }
      update_nivel(rel, rel->nivel + 1);
      char filho_prefixo[strlen(rel->prefixo)];
      strcpy(filho_prefixo, rel->prefixo);
      update_prefixo(rel, (filho_prefixo + strlen(match)));
      anexar_filho(paiComum, rel);
      n->filhos[relId] = paiComum;
      free((void *)match);
      return;
    }

  } else {
    // Se nao tiver nenhum relevante, cria um nó diretamente abaixo 
    add_filho(n, chave, 1, valor);
    return;
  }

  printf("ERRO: CASO DESCONHECIDO DE INSERÇÃO \"%s\"\n", chave);
}


Node *novo_no(const char *prefixo, int ehNoFolha, int valor) {
  //Aloca memoria para o novo nó
  Node *n = (Node *)calloc(1, sizeof(Node));
  //Cada vez que um novo nó é criado, é atribuido um id
  n->id = contador_no;
  //Se contiver um prefixo, ele é copiado para o nó
  if (prefixo == NULL) {
    n->prefixo= NULL;
  } else {
    //Aloca memoria memoria para o prefixo
    n->prefixo = (char *)calloc(strlen(prefixo) + 1, sizeof(char));
    strcpy(n->prefixo, prefixo);
  }
  //Inicialização dos contadores do nó
  n->contadorFilho = 0; //Quantidade de filhos
  n->nivel = 0; //nivel
  n->ehNoFolha = ehNoFolha; //Define se é folha, contendo uma chave ou nao
  n->valor = valor; //Se for folha, armazena o valor
  contador_no++; //Contador global de nós
  return n;
}

//Adiciona um novo nó filho ao nó pai
void add_filho(Node *noPai, const char *prefixo, int ehNoFolha, int valor) {
  //Contador de filhos do nó pai
  noPai->contadorFilho++;
  //Verifica se é o primeiro filho, alocando memoria para o ponteiro
  if (noPai->contadorFilho == 1) {
    noPai->filhos = (Node **)calloc(1, sizeof(Node *));
  } else {
    //Caso nao seja o primeiro, realoca memoria para armazendo o ponteiro do novo filho
    Node **ptr = realloc(noPai->filhos, sizeof(Node*) * noPai->contadorFilho);
    if (ptr == NULL) {
      printf("FATAL: FALHA AO REALOCAR FILHO #%d\n", noPai->id);
    } else {
      noPai->filhos = ptr;
    }
  }
  //Cria o novo nó com suas flags
  Node *filho = novo_no(prefixo, ehNoFolha, valor);
  //Define o nivel do nó filho, um abaixo do nó pai
  filho->nivel = noPai->nivel + 1;
  //Adiciona ponteiro do filho para a array de filhos do nó pai
  noPai->filhos[noPai->contadorFilho - 1] = filho;
}

//Anexar um filho a lista de filhos do pai, para garantir a compactação
void anexar_filho(Node *noPai, Node *filho) {
  //Incremente o contador de filhos do nó pai
  noPai->contadorFilho++;
  //Verifica se é o primeiro filho, alocando memoria para o ponteiro
  if (noPai->contadorFilho == 1) {
    noPai->filhos = (Node **)malloc(sizeof(Node *));
  } else {
    //Caso contrario, realoca a memoria para o novo ponteiro
    Node **ptr = realloc(noPai->filhos, sizeof(Node *) * noPai->contadorFilho);
    if (ptr == NULL) {
      printf("FATAL: FALHA AO REALOCAR NO #%d\n", noPai->id);
    } else {
      noPai->filhos = ptr;
    }
  }
  //Define nivel do filho um abaixo do nó pai
  filho->nivel = noPai->nivel + 1;
  //Adiciona ponteiro do filho para a array de filhos do nó pai
  noPai->filhos[noPai->contadorFilho - 1] = filho;
}




//-----------------------------------------------------FUNÇÃO ENCONTRAR------------------------------------------//
//Encontrar chave na arvore
const int* encontrar_arvore(PatriciaTree* arvore, const char* chave) {
   Node* node = encontrar(arvore->raiz, chave);
   return node ? &(node->valor) : NULL;
}


//Encontrar chave
Node* encontrar(Node *n, const char *chave) {
  printf("> \"%s\"\n", chave);
  //Itera sobre todos os filhos do nó 
  for (int i = 0; i < n->contadorFilho; i++) {
    //Verifica se o nó atual é igual a chave e se é um nó folha
    if (strcmp(n->filhos[i]->prefixo, chave) == 0) {
      return n->filhos[i];
    } else {
      //Caso contrario, verifica se o filho do nó é igual a chave. Recursivamente
      if (strcnt(chave, n->filhos[i]->prefixo)) {
        return encontrar(n->filhos[i], (chave + strlen(n->filhos[i]->prefixo)));
      }
    }
  }
  //Chave nao foi encontrada
  return NULL;
}


// -------------------------------------------------- FUNÇÃO REMOVER -----------------------------------------------//
//Remove um nó da arvore
void remover_no(PatriciaTree *arvore, Node *pai, Node *filho) {
    //Verifica se a chave existe
    if (filho == NULL) {
        printf("OBS: CHAVE NAO ENCONTRADA.\n");
        return;
    }
    //Procura o index do nó a ser removido dentro da array de filhos do pai
    int index = -1;
    for (int i = 0; i < pai->contadorFilho; i++) {
        if (pai->filhos[i] == filho) {
            index = i;
            break;
        }
    }
    //Se for encontrado o index
    if (index != -1) {
        // Remove o nó da lista de filhos do pai
        for (int i = index; i < pai->contadorFilho - 1; i++) {
            pai->filhos[i] = pai->filhos[i + 1];
        }
        pai->contadorFilho--;

        // Libera a memória associada ao nó
        free_no(filho);

        // Se o pai tiver apenas um filho restante e não for um nó folha,
        // mescla o pai com seu filho
        if (pai->contadorFilho == 1 && pai != arvore->raiz && !pai->ehNoFolha) {
            Node *noFilho = pai->filhos[0];
            // Concatena o prefixo do pai com o prefixo do filho
            char *novoPrefixo = (char *)malloc(strlen(pai->prefixo) + strlen(noFilho->prefixo) + 1);
            strcpy(novoPrefixo, pai->prefixo);
            strcat(novoPrefixo, noFilho->prefixo);
            free(pai->prefixo);
            pai->prefixo = novoPrefixo;
            // Atualiza as propriedades do pai com as propriedades do filho
            pai->ehNoFolha = noFilho->ehNoFolha;
            pai->valor = noFilho->valor;
            // Atualiza o contador de filhos do pai
            pai->contadorFilho = noFilho->contadorFilho;
            // Aloca espaço para os filhos do pai
            pai->filhos = (Node **)malloc(sizeof(Node *) * pai->contadorFilho);
            // Copia os filhos do filho para os filhos do pai
            for (int i = 0; i < pai->contadorFilho; i++) {
                pai->filhos[i] = noFilho->filhos[i];
                // Atualiza o nível dos filhos do filho
                pai->filhos[i]->nivel = pai->nivel + 1;
            }
            // Libera o nó filho sem liberar seu array de filhos (agora adotado pelo pai)
            free(noFilho->prefixo);
            free(noFilho);
        }
    } else {
        printf("OBS: NAO ENCONTRADO NÓ FILHO.\n");
    }
}


void remover_no_arvore(PatriciaTree* arvore, const char* chave) {
    //Verificação da chave
    if (strlen(chave) == 0) {
        printf("OBS: TENTANDO REMOVER CHAVE VAZIA.\n");
        return;
    }
    //Localizar o no correspondente a chave
    Node *no_para_remover = encontrar_no(arvore->raiz, chave);
    //Se for encontrado e for um nó folha, remove
    if (no_para_remover != NULL && no_para_remover->ehNoFolha) {       
        remover_no(arvore, encontrar_pai(arvore->raiz, no_para_remover), no_para_remover);
        printf("Chave \"%s\" removida.\n", chave);
    } else {
        printf("OBS: CHAVE \"%s\" NAO ENCONTRADA.\n", chave);
    }
}

Node *encontrar_pai(Node *raiz, Node *filho) {
  //Se a raiz da subarvore da busca for igual a nulo ou é igual ao filho,
  //significa que o nó filho é a propria raiz, nao tendo pai
  if (raiz == NULL || raiz == filho) {
    return NULL;
  }
  //Itera sobre todos os filhos da raiz, verificando se é igual ao nó filho passado. A raiz é o pai
  for (int i = 0; i < raiz->contadorFilho; i++) {
    if (raiz->filhos[i] == filho) {
      return raiz;
    } else {
      //Caso contrario, chama recursivamente para procurar o pai
      Node *pai = encontrar_pai(raiz->filhos[i], filho);
      if (pai != NULL) {
        return pai;
      }
    }
  }
  return NULL;
}

//Buscar nó com base na chave 
Node* encontrar_no(Node *n, const char *chave) {
    //Itera sobre todos os filhos, compara o prefixo e verifica se é igual a chave procurada
    for (int i = 0; i < n->contadorFilho; i++) {
        if (strcmp(n->filhos[i]->prefixo, chave) == 0) {
            return n->filhos[i];
        } else {
            //Se nao encontrar precisa verificar os filhos do filho, e chama recursivamente
            if (strcnt(chave, n->filhos[i]->prefixo)) {
                return encontrar_no(n->filhos[i], (chave + strlen(n->filhos[i]->prefixo)));
            }
        }
    }
    return NULL;
}




// --------------------------------------- FUNÇÕES AUXILIARES ---------------------------------------------------------//
// Retorna 1 se 'chave' esta na arvore, 0 se nao esta
int temchave_arvore(PatriciaTree* arvore, const char* chave) {
  return encontrar_arvore(arvore, chave) ? 1 : 0;
}

// Retorna numero total de chaves na arvore
int contador_chave(PatriciaTree* arvore) {
  return arvore->contador_chaves;
}

// Compara duas strings e volta 'result' como ate onde tem em comum (prefixo)
void strmtc(const char *a, const char *b, char *result) {
  int i = 0;
  while (a[i] == b[i] && a[i] != 0) {
    i++;
  }
  if (i > 0) {
    strncpy(result, a, i);
  }
}

// Percorre duas string e verifica se sao realmente iguais
int strcnt(const char *a, const char *b) {
  int i = 0;
  for (i = 0; a[i] != 0 && b[i] != 0; i++) {
    if (a[i] != b[i]) {
      return 0;
    }
  }
  if (a[i] == b[i]) {
    return 1;
  }
  return a[i] == 0 ? 0 : 1;
}

// Atualiza o nivel do nó na arvore e seus descendentes 
void update_nivel(Node *n, int nivel) {
  n->nivel = nivel;
  for (int i = 0; i < n->contadorFilho; i++) {
    update_nivel(n->filhos[i], nivel + 1);
  }
}

//Atualiza o prefixo em comum
void update_prefixo(Node *n, const char *prefixo) {
  free((void *)n->prefixo);
  n->prefixo = (char *)calloc(strlen(prefixo) + 1, sizeof(char));
  strcpy(n->prefixo, prefixo);
}




//------------------------------------------------------PRINTAR ARVORE----------------------------------------//
void pPrint_arvore(PatriciaTree* arvore) {
  if(arvore) {
    print_arvore(arvore->raiz);
  }
}

void print_arvore(Node *n) {
  print_no(n);
  for (int i = 0; i < n->contadorFilho; i++) {
    print_arvore(n->filhos[i]);
  }
}

void print_no(Node *n) {
  for(int i = 0; i < n->nivel; i++) {
    printf("   ");
  }
  printf("| %s\n", n->prefixo == NULL ? "RAIZ" : n->prefixo);
}



//-----------------------------------------------DESTRUIR ARVORE--------------------------------------------------------//
//Liberar toda a memoria alocada para a arvore
void free_arvore(PatriciaTree* arvore) {
  free_no(arvore->raiz);
  free(arvore);
}

//Libera memoria do nó, percorrendo todos os filhos
void free_no(Node *n) {
  for (int i = 0; i < n->contadorFilho; i++) {
    free_no(n->filhos[i]);
  }
  free((void *)n->prefixo);
  free((void *)n->filhos);
  free((void *)n);
}