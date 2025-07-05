#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    Le caminhos de um txt e coloca em uma arvore:
*/
int uploadArvore(Arvore* raiz, char* caminhoArquivo) {
    if (raiz == NULL)
        return 0;

    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
        return 0;

    char linha[QTD_CARACTERES_LEITURA];

    while (fscanf(arquivo, " %[^\n]", linha) == 1) { // Lembrar mudar (tentar tirar o 1)
        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) == 0)
            continue;

        // Fazer cópia do caminho porque strtok modifica a string
        char caminho[QTD_CARACTERES_LEITURA];
        strcpy(caminho, linha);

        char *barra = strtok(caminho, "/");
        char caminhoCompleto[QTD_CARACTERES_LEITURA] = "";

        while (barra != NULL) {
            if (strlen(caminhoCompleto) > 0)
                strcat(caminhoCompleto, "/");
            strcat(caminhoCompleto, barra);

            // Verifica se já existe
            if (search(raiz, caminhoCompleto) == NULL) {
                if (strchr(barra, '.') != NULL) {
                    mkarq(raiz, caminhoCompleto);
                } else {
                    mkdir(raiz, caminhoCompleto);
                }
            }

            barra = strtok(NULL, "/");
        }
    }

    fclose(arquivo);
    return 1;
}

/*
    Função auxiliar para remoções recursivas, de um nó e toda sua sub-arvore:
*/
void removeRec(NO* no) {
    if(no == NULL) return;

    removeRec(no->filho);
    removeRec(no->irmao);

    free(no->caminho);
    free(no->nome);
    free(no->extensao);
    free(no);
}

/*
    Verifica se existe um arquivo por um caminho completo:
*/
NO* searchPorCaminho(Arvore* raiz, char* caminhoCompleto) {
    if (raiz == NULL || *raiz == NULL || caminhoCompleto == NULL)
        return NULL;

    NO *pilha[QTD_CARACTERES_LEITURA];
    int topo = 0;
    pilha[topo++] = *raiz;

    while (topo > 0) {
        NO *atual = pilha[--topo];

        if (atual->caminho != NULL && strcmp(atual->caminho, caminhoCompleto) == 0)
            return atual;

        if (atual->irmao)
            pilha[topo++] = atual->irmao;
        if (atual->filho)
            pilha[topo++] = atual->filho;
    }

    return NULL;
}

/*
    Busca pelo pai de um arquivo ou pasta:
*/
NO* buscarPai(Arvore* raiz, char* caminhoCompleto) {
    if (caminhoCompleto == NULL || raiz == NULL || *raiz == NULL)
        return NULL;

    char caminhoPai[QTD_CARACTERES_LEITURA];
    strcpy(caminhoPai, caminhoCompleto);

    char *barra = strrchr(caminhoPai, '/');
    if (barra != NULL) {
        *barra = '\0'; // corta o último componente para pegar o pai
    } else {
        // Não tem pai (raiz)
        return NULL;
    }

    // Se caminhoPai ficar vazio, significa que o pai é raiz
    if (strlen(caminhoPai) == 0)
        return *raiz;

    return (NO*) searchPorCaminho(raiz, caminhoPai);
}

/*
    Insere um NÓ (Arquivo ou Pasta):
*/
void inserirNo(Arvore* raiz, NO* pai, NO* novoNo) {
    if (pai != NULL) {
        if (pai->filho == NULL) {
            pai->filho = novoNo;
        } else {
            NO *irm = pai->filho;
            while (irm->irmao != NULL)
                irm = irm->irmao;
            irm->irmao = novoNo;
        }
    } else {
        if (*raiz == NULL) {
            *raiz = novoNo;
        } else {
            NO *atual = *raiz;
            while (atual->irmao != NULL)
                atual = atual->irmao;
            atual->irmao = novoNo;
        }
    }
}

/*
    Libera a arvore toda:
*/
void liberarArvore(Arvore* raiz) {
    if (raiz == NULL || *raiz == NULL)
        return;

    removeRec(*raiz); // Libera recursivamente todos os nós
    *raiz = NULL;     // Garante que o ponteiro da raiz aponte para NULL
}

//////////////////////////////////////////////////////////////////////////////////
// Funções de usuário ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Entra no diretório especificado se ele existir, se ele não existe,
    então imprimi as possíveis alternativas, Ex: diretório = “Me” deve
    informar que existe um diretório “Meus Documentos” e “Meus Downloads”
    senão existe alternativas então imprimi “Diretório não encontrado”:
*/
Arvore cd(NO* atual, char* diretorio) {
    if (atual == NULL || diretorio == NULL) {
        printf("Diretório não encontrado.\n");
        return NULL;
    }

    // Caso "." → permanece
    if (strcmp(diretorio, ".") == 0) {
        return atual;
    }

    if (strcmp(diretorio, "..") == 0) {
        return atual->pai ? atual->pai : atual;
    }
    NO* filho = atual->filho;
    while(filho != NULL){
        // Verifica se o nome do nó atual é igual ao diretório
        if (filho->nome != NULL && filho->extensao != NULL && strcmp(filho->nome, diretorio) == 0) {
            return filho; // Retorna o nó atual se for o diretório desejado
        }

        filho = filho->irmao; // Muda para o irmão
    }

    return atual;
}

/*
    Busca um arquivo ou pasta pelo seu nome “arg” e informa a sua localização:
*/
char* search(Arvore* raiz, char* arg) {
    if (raiz == NULL || *raiz == NULL || arg == NULL) 
        return NULL;

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
    Remove um pasta e seus arquivos, deve fazer uma liberação recursiva:
*/
int rm(Arvore* raiz, char* diretorio) {
    if (raiz == NULL || *raiz == NULL || diretorio == NULL) 
        return 0;

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
    Lista todos os componentes dentro da pasta atual:
*/
int list(Arvore* raiz, char* diretorio) {

    return 0;
}

/*
    Cria uma pasta com o nome “arg” na pasta atual:
*/
int mkdir(Arvore* raiz, char* caminhoCompleto) {
    if (raiz == NULL || caminhoCompleto == NULL)
        return 0;

    // Extrai o nome do diretório do caminho
    char *nome = strrchr(caminhoCompleto, '/');
    nome = (nome != NULL) ? nome + 1 : caminhoCompleto;

    // Cria o novo nó
    NO *no = (NO*) malloc(sizeof(NO));
    if (no == NULL)
        return 0;

    no->caminho = strdup(caminhoCompleto);
    no->nome = strdup(nome);
    no->extensao = NULL; // Pasta não tem extensão
    no->filho = NULL;
    no->irmao = NULL;
    no->pai = NULL;

    // Buscar pai
    NO* pai = buscarPai(raiz, caminhoCompleto);
    no->pai = pai;

    inserirNo(raiz, pai, no);

    return 1;
}

/*
    Sair do Programa e liberar a Árvore:
*/
int exitPrograma(Arvore* raiz) {
    if (raiz == NULL || *raiz == NULL) {
        printf("Árvore não inicializada.\n");
        return 0;
    }
    liberarArvore(raiz);
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////
// Funções Extras  ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Cria um arquivo:
*/
int mkarq(Arvore* raiz, char* caminhoCompleto) {
    if (raiz == NULL || caminhoCompleto == NULL)
        return 0;

    char *nome = strrchr(caminhoCompleto, '/');
    nome = (nome != NULL) ? nome + 1 : caminhoCompleto;

    // Detectar extensão
    char *extensao = strrchr(nome, '.');
    if (extensao == NULL)
        return 0; 

    NO *no = (NO*) malloc(sizeof(NO));
    if (no == NULL)
        return 0;

    no->caminho = strdup(caminhoCompleto);
    no->nome = strdup(nome);
    no->extensao = strdup(extensao); // Inclui o ponto
    no->filho = NULL;
    no->irmao = NULL;
    no->pai = NULL;

    // Encontrar pai
    NO* pai = buscarPai(raiz, caminhoCompleto);
    no->pai = pai;

    inserirNo(raiz, pai, no);

    return 1;
}

//////////////////////////////////////////////////////////////////////////////////
// Terminal  /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Inicia o terminal:
*/
void terminal(Arvore* raiz) {
    if (raiz == NULL) {
        printf("Árvore não inicializada.\n");
        exitPrograma(raiz);
        return;
    }
    NO* atual = *raiz;
    char linha[256];
    char *comando, *arg;

    printf("-------- Bem-vindo ao terminal! --------\n");
    printf("Digite 'help' para ver os comandos disponíveis.\n");

    int sair = 0;
    while (sair == 0) {
        // Prompt
        system("cls");
        if (atual != NULL && atual->caminho != NULL) {
            printf("[%s] $ ", atual->caminho);
        } else {
            printf("[raiz] $ ");
        }

        // Lê a linha inteira
        if (!fgets(linha, sizeof(linha), stdin)) {
            // EOF ou erro de leitura
            printf("\n");
            continue;
        }

        // Remove o '\n' final, se presente
        linha[strcspn(linha, "\n")] = '\0';

        // Separa em tokens: comando e, opcionalmente, argumento
        comando = strtok(linha, " \t");
        arg = strtok(NULL, "\t");

        if (comando == NULL) {
            // Linha em branco: apenas repete o prompt
            continue;
        }

        // Comandos
        if (strcmp(comando, "cd") == 0) {
            if (arg == NULL) {
                printf("Uso: cd <diretório>\n");
            } else {
                atual = cd(atual, arg);
            }
        }
        else if (strcmp(comando, "list") == 0) {
            // list(atual, arg);    // se ls não precisar de arg, você pode passar NULL
        }
        else if (strcmp(comando, "mkdir") == 0) {
            if (arg == NULL) {
                printf("Uso: mkdir <nome>\n");
            } else {
                mkdir(&atual, arg);
            }
        }
        else if (strcmp(comando, "rm") == 0) {
            if (arg == NULL) {
                printf("Uso: rm <nome>\n");
            } else {
                rm(&atual, arg);
            }
        }
        else if (strcmp(comando, "search") == 0) {
            if (arg == NULL) {
                printf("Uso: search <termo>\n");
            } else {
                if(search(&atual, arg) != NULL) {
                    printf("Arquivo ou pasta encontrado: %s\n", search(&atual, arg));
                } else {
                    printf("Arquivo ou pasta não encontrado.\n");
                }
            }
        }
        else if (strcmp(comando, "help") == 0) {
            help();
        }
        else if (strcmp(comando, "exit") == 0) {
            liberarArvore(raiz); // Libera a memória da árvore antes de sair
            exitPrograma(raiz);
            break;
        }
        else {
            printf("Comando '%s' não reconhecido. Digite 'help' para ver os disponíveis.\n", comando);
        }
        system("pause");
    }

    liberarArvore(raiz);
    printf("Saindo do terminal.\n");
}

/*
    Exibe o menu de ajuda.
*/
void help() {
    printf("Comandos disponíveis:\n");
    printf(" - cd <diretório> - Muda para o diretório especificado.\n");
    printf(" - ls - Lista os arquivos e pastas no diretório atual.\n");
    printf(" - mkdir <nome> - Cria um novo diretório.\n");
    printf(" - rm <nome> - remove  um  pasta  e  seus  arquivos,  deve  fazer  uma  liberação recursiva.\n");
    printf(" - search <nome> - busca  um  arquivo  ou  pasta  pelo  seu  nome  “arg”.\n");
    printf(" - clear - Limpa a tela do terminal.\n");
    printf(" - help - Exibe este menu de ajuda.\n");
    printf(" - exit - Sai do terminal.\n");
    printf("\n------------------------------------------\n");
}