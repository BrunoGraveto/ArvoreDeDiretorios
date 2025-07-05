#ifndef ARVORE_H
#define ARVORE_H

// Struct Nó
typedef struct NO {
    char *caminho;
    char *nome;
    char *extensao;
    struct NO *pai; // Pasta pai (nivel acima do atual)
    struct NO *filho; // Arquivos no proximo nivel (em caso de pasta)
    struct NO *irmao; // Arquivo ao lado do nó atual
} NO;

// Struct Arvore
typedef struct NO* Arvore;

/*
    Funções Internas:
*/  
Arvore* criarArvore();
int uploadArvore(Arvore* raiz, char* caminhoArquivo);
void removeRec(NO* no);
NO* searchPorCaminho(Arvore* raiz, char* caminhoCompleto);
NO* buscarPai(Arvore* raiz, char* caminhoCompleto);
void inserirNo(Arvore* raiz, NO* pai, NO* novoNo);

/*
    Funções de Usuário:
*/  
Arvore cd(NO* atual, char* diretorio);
char* search(Arvore* raiz, char* arg);
int rm(Arvore* raiz, char* diretorio);
int list(Arvore* raiz);
int mkdir(Arvore* raiz, char* caminhoCompleto);
int exitPrograma(Arvore* raiz);

/*
    Funções Extras:
*/  
int mkarq(Arvore* raiz, char* caminhoCompleto);

/*
    Funções do Terminal:
*/  
void terminal(Arvore* raiz);
void help();
void liberarArvore(Arvore* raiz); 

#endif