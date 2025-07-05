#include "arvore.h"

int main() {
    
    Arvore *arvore = criarArvore();
    if (arvore == NULL) {
        printf("Erro ao criar a árvore.\n");
        return 1;
    }
    terminal(arvore);
    return 0;
}
