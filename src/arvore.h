#ifndef ARVORE_H
#define ARVORE_H

typedef struct NO* Arvore;

Arvore* criarArvore();
void liberarArvore(Arvore *raiz);
int inserirArvore(Arvore* raiz, int valor);
int removerArvore(Arvore *raiz, int valor);
struct NO* removerNoAtual(struct NO* atual);
int arvoreVazia(Arvore *raiz);
int alturaArvore(Arvore *raiz);
int totalNosArvore(Arvore *raiz);
int consultarArvore(Arvore *raiz, int valor);
void preOrdemArvore(Arvore *raiz);
void emOrdemArvore(Arvore *raiz);
void posOrdemArvore(Arvore *raiz);

#endif