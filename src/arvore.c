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

//////////////////////////////////////////////////////////////////////////////////
// Funções de usuário ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Entra no diretório especificado se ele existir, se ele não existe,
    então imprimi as possíveis alternativas, Ex: diretório = “Me” deve
    informar que existe um diretório “Meus Documentos” e “Meus Downloads”
    senão existe alternativas então imprimi “Diretório não encontrado”
*/
Arvore cd(Arvore atual, char* diretorio) {
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
    Arvore filho = atual->filho;
    while(filho != NULL){
        // Verifica se o nome do nó atual é igual ao diretório
        if (filho->nome != NULL && strcmp(filho->nome, diretorio) == 0) {
            return filho; // Retorna o nó atual se for o diretório desejado
        }

        filho = filho->irmao; // Muda para o irmão
    }

    return NULL;
}

/*
    Busca um arquivo ou pasta pelo seu nome “arg” e informa a sua localização.
*/
char* search(Arvore* raiz, char* arg) {
    if (raiz == NULL || *raiz == NULL || arg == NULL) return NULL;

    NO* atual = *raiz;
    NO* ult = NULL; // Último nó visitado, usado para evitar voltar ao mesmo nó

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

/*
    Limpa a tela do terminal.
    Dependendo do sistema operacional, usa "cls" ou "clear".
*/
int clear() {
    // Limpa a tela do terminal
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    return 1;
}

/*
    Sai do terminal e libera a memória da árvore.
*/
int exit(Arvore* raiz) {
    if (raiz == NULL || *raiz == NULL) {
        printf("Árvore não inicializada.\n");
        return 0;
    }
    liberarArvore(raiz);
    return 1;
}



/*
    Função que inicia o loop terminal, deve ser chamada no main.
    comando\0arg\0
*/
void terminal(Arvore* raiz) {
    if (raiz == NULL) {
        printf("Árvore não inicializada.\n");
        exit(1);
    }
    Arvore atual = raiz;
    char linha[256];
    char *comando, *arg;

    printf("-------- Bem-vindo ao terminal! --------\n");
    printf("Digite 'help' para ver os comandos disponíveis.\n");

    while (1) {
        // Prompt
        printf("[%s] $ ", atual->caminho ? atual->caminho : "raiz");

        // Lê a linha inteira
        if (!fgets(linha, sizeof(linha), stdin)) {
            // EOF ou erro de leitura
            printf("\n");
            break;
        }

        // Remove o '\n' final, se presente
        linha[strcspn(linha, "\n")] = '\0';

        // Separa em tokens: comando e, opcionalmente, argumento
        comando = strtok(linha, " \t");
        arg     = strtok(NULL,  " \t");

        if (comando == NULL) {
            // Linha em branco: apenas repete o prompt
            continue;
        }

        // Comandos
        if (strcmp(comando, "cd") == 0) {
            if (arg == NULL) {
                printf("Uso: cd <diretório>\n");
            } else {
                if ((atual = cd(atual, arg)) == NULL) {
                    printf("Diretório '%s' não encontrado.\n", arg);
                } else {
                    printf("Mudando para o diretório: %s\n", atual->caminho ? atual->caminho : "raiz");
                }
            }
        }
        else if (strcmp(comando, "ls") == 0) {
            list(atual, arg);    // se ls não precisar de arg, você pode passar NULL
        }
        else if (strcmp(comando, "mkdir") == 0) {
            if (arg == NULL) {
                printf("Uso: mkdir <nome>\n");
            } else {
                mkdir(atual, arg);
            }
        }
        else if (strcmp(comando, "rm") == 0) {
            if (arg == NULL) {
                printf("Uso: rm <nome>\n");
            } else {
                rm(atual, arg);
            }
        }
        else if (strcmp(comando, "search") == 0) {
            if (arg == NULL) {
                printf("Uso: search <termo>\n");
            } else {
                if(search(atual, arg) != NULL) {
                    printf("Arquivo ou pasta encontrado: %s\n", search(atual, arg));
                } else {
                    printf("Arquivo ou pasta não encontrado.\n");
                }
            }
        }
        else if (strcmp(comando, "help") == 0) {
            help();
        }
        else if (strcmp(comando, "clear") == 0) {
            clear(); // Limpa a tela do terminal
        }
        else if (strcmp(comando, "exit") == 0) {
            exit(raiz);
        }
        else {
            printf("Comando '%s' não reconhecido. Digite 'help' para ver os disponíveis.\n", comando);
        }
    }

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

//////////////////////////////////////////////////////////////////////////////////
// Funções Extras  ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
