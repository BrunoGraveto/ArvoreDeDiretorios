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

    while (fscanf(arquivo, " %[^\n]", linha) == 1) {
        char caminho[QTD_CARACTERES_LEITURA];
        strcpy(caminho, linha);

        char* parte = strtok(caminho, "/");
        char caminhoParcial[QTD_CARACTERES_LEITURA] = "";

        while (parte != NULL) {
            if (strlen(caminhoParcial) > 0)
                strcat(caminhoParcial, "/");
            strcat(caminhoParcial, parte);

            NO* encontrado = searchPorCaminho(raiz, caminhoParcial);
            if (encontrado == NULL) {
                if (strchr(parte, '.') != NULL) {
                    mkarq(raiz, caminhoParcial);
                } else {
                    mkdir_(raiz, caminhoParcial);
                }
            }

            parte = strtok(NULL, "/");
        }
    }

    fclose(arquivo);
    return 1;
}


/*
    Função auxiliar para remoções recursivas, de um nó e toda sua sub-arvore:
*/
void removeRec(NO* no) {
    if (!no) return;
    // Primeiro libera toda a sub-árvore de filhos
    removeRec(no->filho);
    // Depois libera os irmãos daquele ramo
    removeRec(no->irmao);
    // Por fim, libera os campos alocados e o próprio nó
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
    senão existe alternativas então imprimi “Diretório não encontrado”
*/
NO* cd(NO* atual, char* diretorio) {
    if (atual == NULL || diretorio == NULL) {
        printf("Diretorio nao encontrado.\n");
        return NULL;
    }

    // Caso "." → permanece
    if (strcmp(diretorio, ".") == 0) return atual;
    if (strcmp(diretorio, "..") == 0) return atual->pai ? atual->pai : atual;
    
    NO* filho = atual->filho;
    while(filho != NULL){
        // Verifica se o nome do nó atual é igual ao diretório
        if (filho->nome != NULL && strcmp(filho->nome, diretorio) == 0 && filho->extensao == NULL) {
            return filho; // Retorna o nó atual se for o diretório desejado
        }

        filho = filho->irmao; // Muda para o irmão
    }
    printf("Diretorio nao encontrado\n");
    return atual;
}

/*
    Busca um arquivo ou pasta pelo seu nome “arg” e informa a sua localização:
*/
char* search(Arvore* raiz, char* arg) {
    if (raiz == NULL || *raiz == NULL || arg == NULL)
        return NULL;

    NO* atual = *raiz;

    while (atual != NULL) {
        if (atual->nome != NULL && strcmp(atual->nome, arg) == 0)
            return atual->caminho;

        if (atual->filho != NULL) {
            atual = atual->filho;
        } else if (atual->irmao != NULL) {
            atual = atual->irmao;
        } else {
            // Volta até encontrar um nó com irmão não visitado
            while (atual != NULL && atual->irmao == NULL)
                atual = atual->pai;
            if (atual != NULL)
                atual = atual->irmao;
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

    // ponteiro para o ponteiro do primeiro filho
    NO** ptr = &((*raiz)->filho);

    // itera sobre a lista de filhos
    while (*ptr) {
        NO* filho = *ptr;
        if (filho->nome && strcmp(filho->nome, diretorio) == 0) {
            // desconecta o nó
            *ptr = filho->irmao;
            filho->irmao = NULL;
            // libera todo o ramo
            removeRec(filho);
            return 1;
        }
        // avança para o próximo irmão
        ptr = &(filho->irmao);
    }
    return 0;
}

/*
    Lista todos os componentes dentro da pasta atual:
*/
int list(Arvore* raiz) {
    if(raiz == NULL || *raiz == NULL) {
        printf("a");
        return 0;
    }
    char* diretorio = (*raiz)->nome ? (*raiz)->nome : "raiz"; // Se não tiver nome, assume raiz

    NO* atual = *raiz;

    printf("\n%s/\n", diretorio);
    NO* filho = atual->filho;
    if (filho == NULL) {
        printf("Diretorio vazio.\n");
    }
    while (filho != NULL) {
        printf("    %s\n", filho->nome);
        filho = filho->irmao;
    }
    printf("\n");
    return 1;
}

/*
    Cria uma pasta com o nome “arg” na pasta atual:
*/
int mkdir_(Arvore* raiz, char* caminhoCompleto) {
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
    Exibe o menu de ajuda.
*/
void help() {
    printf("Comandos disponiveis:\n");
    printf(" - cd <diretorio> - Muda para o diretorio especificado.\n");
    printf(" - list - Lista os arquivos e pastas no diretorio atual.\n");
    printf(" - mkdir <nome> - Cria um novo diretorio.\n");
    printf(" - rm <nome> - remove  um  pasta  e  seus  arquivos,  deve  fazer  uma  liberacao recursiva.\n");
    printf(" - search <nome> - busca  um  arquivo  ou  pasta  pelo  seu  nome.\n");
    printf(" - clear - Limpa a tela do terminal.\n");
    printf(" - help - Exibe este menu de ajuda.\n");
    printf(" - echo <mensagem> - Exibe a mensagem especificada.\n");
    printf(" - rename <nome_antigo> <nome_novo> - Renomeia um arquivo ou diretorio.\n");
    printf(" - exit - Sai do terminal.\n");
    printf("\n------------------------------------------\n");
    
}

//////////////////////////////////////////////////////////////////////////////////
// Terminal  /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
    Inicia o terminal:
*/
void terminal(Arvore* raiz) {
    if (raiz == NULL) {
        printf("Árvore nao inicializada.\n");
        exitPrograma(raiz);
        return;
    }
    NO* atual = *raiz;
    char linha[256];
    char *comando, *arg;

    printf("-------- Bem-vindo ao terminal! --------\n");
    printf("Digite 'help' para ver os comandos disponiveis.\n");

    int sair = 0;
    while (sair == 0) {
        // Prompt
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
                printf("Uso: cd <diretorio>\n");
            } else {
                atual = cd(atual, arg);
            }
        }
        else if (strcmp(comando, "list") == 0) {
            list(&atual);    // se ls não precisar de arg, você pode passar NULL
        }
        else if (strcmp(comando, "mkdir") == 0) {
            if (arg == NULL) {
                printf("Uso: mkdir <nome>\n");
            } else {
                mkdir_(&atual, arg);
            }
        }
        else if (strcmp(comando, "rm") == 0) {
            if (arg == NULL) {
                printf("Uso: rm <nome>\n");
            } else {
                if(rm(&atual, arg)) {
                    printf("Pasta ou arquivo '%s' removido com sucesso.\n", arg);
                } else {
                    printf("Pasta ou arquivo '%s' não encontrado.\n", arg);
                }
            }
        }
        else if (strcmp(comando, "search") == 0) {
            if (arg == NULL) {
                printf("Uso: search <termo>\n");
            } else {
                if(search(&atual, arg) != NULL) {
                    printf("Arquivo ou pasta encontrado: %s\n", search(&atual, arg));
                } else {
                    printf("Arquivo ou pasta nao encontrado.\n");
                }
            }
        }
        else if (strcmp(comando, "help") == 0) {
            help();
        }
        else if (strcmp(comando, "echo") == 0) {
            if (arg == NULL) {
                printf("Uso: echo <mensagem>\n");
            } else {
                echo(arg);
            }
        }
        else if (strcmp(comando, "rename") == 0) {
            if (arg == NULL) {
                printf("Uso: rename <nome_antigo> <nome_novo>\n");
            } else {
                char* nome_antigo = arg;
                char* nome_novo = strtok(NULL, "\t");
                if (nome_novo == NULL) {
                    printf("Uso: rename <nome_antigo> <nome_novo>\n");
                } else {
                    rename_nome(raiz, nome_antigo, nome_novo);
                }
            }
        }
        else if (strcmp(comando, "clear") == 0) {
            clear();
        }
        else if (strcmp(comando, "exit") == 0) {
            exitPrograma(raiz);
            break;
        }
        else {
            printf("Comando '%s' nao reconhecido. Digite 'help' para ver os disponiveis.\n", comando);
        }
    }

    printf("Saindo do terminal.\n");
}


int rename_nome(Arvore* raiz, char* nome_antigo, char* nome_novo) {
    if (raiz == NULL || *raiz == NULL || nome_antigo == NULL || nome_novo == NULL) {
        printf("Arvore nao inicializada ou nomes invalidos.\n");
        return 0;
    }

    NO* atual = (*raiz)->filho; // Começa do primeiro filho da raiz
    if (atual == NULL) {
        printf("Arvore vazia.\n");
        return 0;
    }
    while (atual != NULL) {
        if (atual->nome != NULL && strcmp(atual->nome, nome_antigo) == 0) {
            free(atual->nome);
            atual->nome = strdup(nome_novo);
            printf("Renomeado '%s' para '%s'.\n", nome_antigo, nome_novo);
            return 1;
        }
        atual = atual->irmao; // Verifica o próximo irmão
    }
    printf("Arquivo ou pasta '%s' nao encontrado.\n", nome_antigo);
    return 0;
}

/*
    Exibe a mensagem especificada.
    Se a mensagem for NULL, imprime "Mensagem inválida."
*/

int echo(char* mensagem) {
    if (mensagem == NULL) {
        printf("Mensagem invalida.\n");
        return 0;
    }
    printf("%s\n", mensagem);
    return 1;
}

/*
    Função para Imprimir todos os arquivos presente em uma pasta, mas com busca global na Arvore
*/

int listAll(Arvore* raiz, char* diretorio) {
    if(raiz == NULL || *raiz == NULL || diretorio == NULL){
        return 0;
    }

    NO* atual = *raiz;
    NO* ult = NULL;

    while(atual != NULL){
        if(strcmp(atual->nome, diretorio) == 0){
            NO* comp = atual->filho;

            if(comp == NULL){
                printf("vazio\n");
                return 1;
            }

            printf("\nArquivos da Pasta %s\n", diretorio);
            while(comp != NULL){
                printf("- %s\n", comp->nome);
                comp = comp->irmao;
            }
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
    return 0;
}