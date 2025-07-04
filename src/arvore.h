#ifndef ARVORE_H
#define ARVORE_H

// Struct Nó
typedef struct NO {
    char *caminho;
    char *nome;
    char extensao;
    NO *pai; // Pasta pai
    NO *filho; // Arquivos
    NO *irmao; // Pastas
} NO;

// Struct Arvore
typedef struct NO* Arvore;

// Funções
int cd(Arvore* raiz, char* diretorio);
char* search(Arvore* raiz, char* arg);
int rm(Arvore* raiz, char* diretorio);
int list(Arvore* raiz, char* diretorio);
int mkdir(Arvore* raiz, char* arg);

#endif