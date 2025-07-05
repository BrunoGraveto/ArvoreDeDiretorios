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
int uploadArvore(Arvore* raiz, char* caminho);
void removeRec (NO* no);//função auxiliar para remoção de pastas e arquivos
void liberarArvore(Arvore* raiz); //Libera a memória alocada para a árvore

// Funções de usuário
NO* cd(NO* atual, char* diretorio);
char* search(Arvore* raiz, char* arg); //busca  um  arquivo  ou  pasta  pelo  seu  nome  “arg”  e  informa  a sua localização
int rm(Arvore* raiz, char* diretorio); //Remove uma pasta e seus arquivos
int list(Arvore* raiz);
int mkdir(Arvore* raiz, char* arg);
void terminal(Arvore* raiz); //Função que inicia o loop terminal
void help(); //Exibe o menu de ajuda
int clear(); //Limpa a tela do terminal


//Funções Extras
int rename(Arvore* raiz, char* nome_antigo, char* nome_novo); //Renomeia um arquivo ou diretório
int echo(char* mensagem); //Exibe a mensagem especificada
int listAll(Arvore* raiz, char* diretorio);

#endif