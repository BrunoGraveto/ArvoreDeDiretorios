#ifndef ARVORE_H
#define ARVORE_H

typedef struct NO* ArvBin;

ArvBin* criarArvore();
void liberarArvore(ArvBin *raiz);
int inserirArvore(ArvBin* raiz, int valor);
int removerArvore(ArvBin *raiz, int valor);
struct NO* removerNoAtual(struct NO* atual);
int arvoreVazia(ArvBin *raiz);
int alturaArvore(ArvBin *raiz);
int totalNosArvore(ArvBin *raiz);
int consultarArvore(ArvBin *raiz, int valor);
void preOrdemArvore(ArvBin *raiz);
void emOrdemArvore(ArvBin *raiz);
void posOrdemArvore(ArvBin *raiz);

#endif