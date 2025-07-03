#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

struct NO {
    char* caminhoArquivo;
    char* nomeArquivo;
    char extensaoArquivo; 
    struct NO *esq;
    struct NO *dir;
};

Arvore* criarArvore(){
    Arvore* raiz = (Arvore*) malloc(sizeof(Arvore));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void liberarNO(struct NO* no){
    if(no == NULL)
        return;
    liberarNO(no->esq);
    liberarNO(no->dir);
    free(no);
    no = NULL;
}

void liberarArvore(Arvore* raiz){
    if(raiz == NULL)
        return;
    liberarNO(*raiz);
    free(raiz);
}

int inserirArvore(Arvore* raiz, int valor){
    if(raiz == NULL)
        return 0;
    struct NO* novo;
    novo = (struct NO*) malloc(sizeof(struct NO));
    if(novo == NULL)
        return 0;
    novo->caminhoArquivo = valor;
    novo->dir = NULL;
    novo->esq = NULL;

    if(*raiz == NULL)
        *raiz = novo;
    else{
        struct NO* atual = *raiz;
        struct NO* ant = NULL;
        while(atual != NULL){
            ant = atual;
            if(valor == atual->caminhoArquivo){
                free(novo);
                return 0;
            }

            if(valor > atual->caminhoArquivo)
                atual = atual->dir;
            else
                atual = atual->esq;
        }
        if(valor > ant->caminhoArquivo)
            ant->dir = novo;
        else
            ant->esq = novo;
    }
    return 1;
}

int removerArvore(Arvore *raiz, int valor){
    if(raiz == NULL)
        return 0;
    struct NO* ant = NULL;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valor == atual->caminhoArquivo){
            if(atual == *raiz)
                *raiz = removerNoAtual(atual);
            else{
                if(ant->dir == atual)
                    ant->dir = removerNoAtual(atual);
                else
                    ant->esq = removerNoAtual(atual);
            }
            return 1;
        }
        ant = atual;
        if(valor > atual->caminhoArquivo)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0;
}

int arvoreVazia(Arvore *raiz){
    if(raiz == NULL)
        return 1;
    if(*raiz == NULL)
        return 1;
    return 0;
}

struct NO* removerNoAtual(struct NO* atual) {
    struct NO *no1, *no2;
    if(atual->esq == NULL){
        no2 = atual->dir;
        free(atual);
        return no2;
    }
    no1 = atual;
    no2 = atual->esq;
    while(no2->dir != NULL){
        no1 = no2;
        no2 = no2->dir;
    }
    if(no1 != atual){
        no1->dir = no2->esq;
        no2->esq = atual->esq;
    }
    no2->dir = atual->dir;
    free(atual);
    return no2;
}

int alturaArvore(Arvore *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = alturaArvore(&((*raiz)->esq));
    int alt_dir = alturaArvore(&((*raiz)->dir));
    if (alt_esq > alt_dir)
        return (alt_esq + 1);
    else
        return(alt_dir + 1);
}

void preOrdemArvore(Arvore *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        printf("%d\n",(*raiz)->caminhoArquivo);
        preOrdemArvore(&((*raiz)->esq));
        preOrdemArvore(&((*raiz)->dir));
    }
}

void emOrdemArvore(Arvore *raiz) {
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        emOrdemArvore(&((*raiz)->esq));
        printf("%d\n",(*raiz)->caminhoArquivo);
        emOrdemArvore(&((*raiz)->dir));
    }
}

void posOrdemArvore(Arvore *raiz) {
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        posOrdemArvore(&((*raiz)->dir));
        posOrdemArvore(&((*raiz)->esq));
        printf("%d\n",(*raiz)->caminhoArquivo);
    }
}

int totalNosArvore(Arvore *raiz) {
    if(raiz == NULL || *raiz == NULL)
        return 0;
    return 1 + totalNosArvore(&((*raiz)->esq)) + totalNosArvore(&((*raiz)->dir));
}

int consultarArvore(Arvore *raiz, int valor) {
    if (raiz == NULL || *raiz == NULL)
        return 0;
    struct NO *atual = *raiz;
    while (atual != NULL) {
        if (atual->caminhoArquivo == valor) {
            return 1;
        }
        if (valor > atual->caminhoArquivo)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0;
}

struct NO* Remove_ArvoreAtual(struct NO* atual) {
    struct NO *no1, *no2;
    if(atual->esq == NULL){
        no2 = atual->dir;
        free(atual);
        return no2;
    }
    no1 = atual;
    no2 = atual->esq;
    while(no2->dir != NULL){
        no1 = no2;
        no2 = no2->dir;
    }
    if(no1 != atual){
        no1->dir = no2->esq;
        no2->esq = atual->esq;
    }
    no2->dir = atual->dir;
    free(atual);
    return no2;
}

///////////////////////////////////////////////////////////////////////

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
    return 0;
}
