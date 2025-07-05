#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

int main() {
    
    Arvore *arvore = criarArvore();
    uploadArvore(arvore, "src/in.txt");
    terminal(arvore);

    system("pause");

    return 0;
}
