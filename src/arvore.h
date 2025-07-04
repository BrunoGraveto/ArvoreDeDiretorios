#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARVORE_H
#define ARVORE_H

// Struct Nó
typedef struct NO {
    char *caminho;
    char *nome;
    char *extensao;
    NO *pai; // Pasta pai (nivel acima do atual)
    NO *filho; // Arquivos no proximo nivel (em caso de pasta)
    NO *irmao; // Arquivo ao lado do nó atual
} NO;

// Struct Arvore
typedef struct NO* Arvore;

// Funções internas
Arvore* criarArvore();
int uploadArquivos(Arvore* raiz, char* caminho);

// Funções de usuário
int cd(Arvore* raiz, char* diretorio);
char* search(Arvore* raiz, char* arg); //busca  um  arquivo  ou  pasta  pelo  seu  nome  “arg”  e  informa  a sua localização
int rm(Arvore* raiz, char* diretorio); //Remove uma pasta e seus arquivos
int list(Arvore* raiz, char* diretorio);
int mkdir(Arvore* raiz, char* arg);

//Funções Extras
//bo adicionar as extrar aqui pra separar e deixar bonitinho

#endif