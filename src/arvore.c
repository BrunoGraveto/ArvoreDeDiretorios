#include "arvore.h"

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
int uploadArquivos(Arvore* raiz, char* caminho) {

}

/*
    Função auxiliar para remoções recursivas, de um nó e toda sua sub-arvore
*/
void removeRec (NO* no){
    if(no == NULL) return;

    removeRec(no->filho);
    removeRec(no->irmao);

    free(no->caminho);
    free(no->nome);
    free(no->extensao);
    free(no);
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
    if (raiz == NULL || *raiz == NULL || arg == NULL) return NULL;

    NO* atual = *raiz;
    NO* ult = NULL; // Último nó visitado

    while (atual != NULL) {
        if (atual->nome != NULL && strcmp(atual->nome, arg) == 0)
            return atual->caminho;
    
        if (atual->filho != NULL && ult != atual->filho) {

            ult = atual;
            atual = atual->filho;

        } else if (atual->irmao != NULL && ult != atual->irmao) {

            ult = atual;
            atual = atual->irmao;

        } else {

            ult = atual;
            atual = atual->pai; // Volta para o pai se não tiver mais filhos ou irmãos
        }
    }

    return NULL;
}

/*
    Remove um pasta e seus arquivos, deve fazer uma liberação recursiva.
*/
int rm(Arvore* raiz, char* diretorio) {
    if (raiz == NULL || *raiz == NULL || diretorio == NULL) {
        return 0;
    }

    NO* atual = *raiz;
    NO* ult = NULL;

    while (atual != NULL) {
        if (strcmp(atual->nome, diretorio) == 0) {
            
            if (atual->pai == NULL) { 
                *raiz = atual->irmao;

            } else {
                
                if (atual->pai->filho == atual) {
                    atual->pai->filho = atual->irmao;

                } else {

                    NO* irmaoAnt = atual->pai->filho;
                    while (irmaoAnt != NULL && irmaoAnt->irmao != atual) {
                        irmaoAnt = irmaoAnt->irmao;
                    }
                    if (irmaoAnt != NULL) {
                        irmaoAnt->irmao = atual->irmao;
                    }
                }
            }
            
            atual->irmao = NULL; 
            removeRec(atual);
            
            return 1;
        }

        if (atual->filho != NULL && ult != atual->filho) {
            ult = atual;
            atual = atual->filho;

        } else if (atual->irmao != NULL && ult != atual->irmao) {
            ult = atual;
            atual = atual->irmao;

        } else {
            ult = atual;
            atual = atual->pai;
        }
    }

    return 0; // Não encontrou
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

//////////////////////////////////////////////////////////////////////////////////
// Funções Extras  ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

