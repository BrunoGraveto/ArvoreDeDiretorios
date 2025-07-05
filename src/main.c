#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

int main() {
    
    Arvore *raiz = criarArvore();
    NO* noRaiz = (NO*) malloc(sizeof(NO));
    noRaiz->nome = strdup("raiz");
    noRaiz->caminho = strdup("raiz");
    noRaiz->extensao = NULL;   
    noRaiz->pai = NULL;    
    noRaiz->filho = NULL;
    noRaiz->irmao = NULL;
    *raiz = noRaiz;
    uploadArvore(raiz, "in.txt");
    terminal(raiz);

    system("pause");

    return 0;
}
