#ifndef ARVORE_H
#define ARVORE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
int uploadArvore(Arvore* raiz, char* caminho);
void removeRec (NO* no);
void liberarArvore(Arvore* raiz); 
NO* buscaCaminhoRecursivo(NO* no, char* caminhoCompleto);
NO* searchPorCaminho(Arvore* raiz, char* caminhoCompleto);

// Funções de usuário
NO* cd(NO* atual, char* caminho);
char* search(Arvore* raiz, char* arg); 
int rm(Arvore* raiz, char* caminho); 
int list(Arvore* raiz);
int mkdir_(Arvore* raiz, char* arg);
void terminal(Arvore* raiz); 
void help();
int exitPrograma(Arvore* raiz);
int clear(); 


//Funções Extras
int rename_nome(Arvore* raiz, char* arg); 
int echo(char* mensagem); 
int listAll(Arvore* raiz, char* caminho);
int mkarq(Arvore* raiz, char* caminhoCompleto);
NO* searchPorCaminho(Arvore* raiz, char* caminhoCompleto);

#endif