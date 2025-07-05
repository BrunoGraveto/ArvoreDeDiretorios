#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

#define QTD_CARACTERES_LEITURA 300

//////////////////////////////////////////////////////////////////////////////////
// Funções Internas //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Criar Arvore:
*/
Arvore* criarArvore() {
    Arvore *raiz = (Arvore*) malloc(sizeof(Arvore));
    if (raiz != NULL)   
        *raiz = NULL;
    return raiz;
}

/*
    Lê varios arquivos de um txt e coloca na arvore e insere
*/
int uploadArvore(Arvore* raiz, char* caminho) {
    if (raiz == NULL) // Lembrar de ver para adicionar verificação de *raiz
        return 0;
    FILE *arquivo = fopen("exemplo.txt", "r");
    if (arquivo == NULL) 
        return 0;
    char linha[QTD_CARACTERES_LEITURA];
    while (fscanf(arquivo, "%[^\n]", linha) != EOF) {

    }
    fclose(arquivo);
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////
// Funções de usuário ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Entra no diretório especificado se ele existir, se ele não existe,
    então imprimi as possíveis alternativas, Ex: diretório = “Me” deve
    informar que existe um diretório “Meus Documentos” e “Meus Downloads”
    senão existe alternativas então imprimi “Diretório não encontrado”
*/
int cd(Arvore* raiz, char* diretorio) {
    return 0;
}

/*
    Busca um arquivo ou pasta pelo seu nome “arg” e informa a sua localização.
*/
char* search(Arvore* raiz, char* arg) {
    return NULL;
}

/*
    Remove um pasta e seus arquivos, deve fazer uma liberação recursiva.
*/
int rm(Arvore* raiz, char* diretorio) {
    return 0;
}   

/*
    Lista todos os componentes dentro da pasta atual.
*/
int list(Arvore* raiz, char* diretorio) {

    return 0;
}

/*
    Cria uma pasta com o nome “arg” na pasta atual.
*/
int mkdir(Arvore* raiz, char* arg) {
    if (raiz == NULL)
        return 0;
    NO *no = (NO*) malloc(sizeof(NO));
    if (no == NULL) 
        return 0;
    
    return 1;
}