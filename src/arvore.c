#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

struct NO {
    int info;
    struct NO *esq;
    struct NO *dir;
};

ArvBin* criarArvore(){
    ArvBin* raiz = (ArvBin*) malloc(sizeof(ArvBin));
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

void liberarArvore(ArvBin* raiz){
    if(raiz == NULL)
        return;
    liberarNO(*raiz);//libera cada n�
    free(raiz);//libera a raiz
}

int inserirArvore(ArvBin* raiz, int valor){
    if(raiz == NULL)
        return 0;
    struct NO* novo;
    novo = (struct NO*) malloc(sizeof(struct NO));
    if(novo == NULL)
        return 0;
    novo->info = valor;
    novo->dir = NULL;
    novo->esq = NULL;

    if(*raiz == NULL)
        *raiz = novo;
    else{
        struct NO* atual = *raiz;
        struct NO* ant = NULL;
        while(atual != NULL){
            ant = atual;
            if(valor == atual->info){
                free(novo);
                return 0;//elemento j� existe
            }

            if(valor > atual->info)
                atual = atual->dir;
            else
                atual = atual->esq;
        }
        if(valor > ant->info)
            ant->dir = novo;
        else
            ant->esq = novo;
    }
    return 1;
}

int removerArvore(ArvBin *raiz, int valor){
    if(raiz == NULL)
        return 0;
    struct NO* ant = NULL;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valor == atual->info){
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
        if(valor > atual->info)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0;
}

int arvoreVazia(ArvBin *raiz){
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

int alturaArvore(ArvBin *raiz){
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

void preOrdemArvore(ArvBin *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        printf("%d\n",(*raiz)->info);
        preOrdemArvore(&((*raiz)->esq));
        preOrdemArvore(&((*raiz)->dir));
    }
}

void emOrdemArvore(ArvBin *raiz) {
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        emOrdemArvore(&((*raiz)->esq));
        printf("%d\n",(*raiz)->info);
        emOrdemArvore(&((*raiz)->dir));
    }
}

void posOrdemArvore(ArvBin *raiz) {
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        posOrdemArvore(&((*raiz)->dir));
        posOrdemArvore(&((*raiz)->esq));
        printf("%d\n",(*raiz)->info);
    }
}

int totalNosArvore(ArvBin *raiz) {
    if(raiz == NULL || *raiz == NULL)
        return 0;
    return 1 + totalNosArvore(&((*raiz)->esq)) + totalNosArvore(&((*raiz)->dir));
}

int consultarArvore(ArvBin *raiz, int valor) {
    if (raiz == NULL || *raiz == NULL)
        return 0;
    struct NO *atual = *raiz;
    while (atual != NULL) {
        if (atual->info == valor) {
            return 1;
        }
        if (valor > atual->info)
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