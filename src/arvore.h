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
void removeRec (NO* no);//função auxiliar para remoção de pastas e arquivos
void liberarArvore(Arvore* raiz); //Libera a memória alocada para a árvore

// Funções de usuário
NO* cd(NO* atual, char* diretorio);
char* search(Arvore* raiz, char* arg); //busca  um  arquivo  ou  pasta  pelo  seu  nome  “arg”  e  informa  a sua localização
int rm(Arvore* raiz, char* diretorio); //Remove uma pasta e seus arquivos
int list(Arvore* raiz);
int mkdir_(Arvore* raiz, char* arg);
void terminal(Arvore* raiz); //Função que inicia o loop terminal
void help(); //Exibe o menu de ajuda
int exitPrograma(Arvore* raiz); //Sai do terminal e libera a memória da árvore
int clear(); //Limpa a tela do terminal


//Funções Extras
int rename_nome(Arvore* raiz, char* nome_antigo, char* nome_novo); //Renomeia um arquivo ou diretório
int echo(char* mensagem); //Exibe a mensagem especificada
int listAll(Arvore* raiz, char* diretorio);
int mkarq(Arvore* raiz, char* caminhoCompleto);
NO* searchPorCaminho(Arvore* raiz, char* caminhoCompleto);

#endif