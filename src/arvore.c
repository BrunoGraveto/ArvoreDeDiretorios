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

    removeRec(no->filho);
    removeRec(no->irmao);

    free(no->caminho);
    free(no->nome);
    free(no->extensao);
    free(no);
}
/*
    Busca um arquivo ou pasta pelo seu caminho completo e retorna seu nó
*/
NO* searchPorCaminho(Arvore* raiz, char* caminhoCompleto) {
    if (raiz == NULL || *raiz == NULL || caminhoCompleto == NULL)
        return NULL;

    return buscaCaminhoRecursivo(*raiz, caminhoCompleto);
}

/*
    Função auxiliar da searhPorCaminnho(recursiva)
*/
NO* buscaCaminhoRecursivo(NO* no, char* caminhoCompleto) {
    if (no == NULL)
        return NULL;

    if (no->caminho != NULL && strcmp(no->caminho, caminhoCompleto) == 0)
        return no;

    // Busca nos filhos
    NO* resultado = buscaCaminhoRecursivo(no->filho, caminhoCompleto);
    if (resultado != NULL)
        return resultado;

    // Busca nos irmãos
    return buscaCaminhoRecursivo(no->irmao, caminhoCompleto);
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

    removeRec(*raiz); 
    *raiz = NULL;     
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
NO* cd(NO* atual, char* caminho) {
    if (atual == NULL || caminho == NULL) {
        printf("caminho nao encontrado.\n");
        return NULL;
    }

    // Casos especiais
    if (strcmp(caminho, ".") == 0) return atual;
    if (strcmp(caminho, "..") == 0) return atual->pai ? atual->pai : atual;
    
    // Busca filho por nome
    NO* filho = atual->filho;
    while(filho != NULL){
        if (filho->nome != NULL && strcmp(filho->nome, caminho) == 0 && filho->extensao == NULL) {
            return filho; 
        }

        filho = filho->irmao; 
    }

    printf("caminho nao encontrado\n");

    // Caso caminho não seja encontrado, imprime possíveis caminhos

    printf("Possiveis caminhos:\n");
    filho = atual->filho;
    if (filho == NULL) {
        printf("caminho vazio.\n");
    }
    while (filho != NULL) {
        printf(" - %s\n", filho->nome);
        filho = filho->irmao;
    }
    printf("\n");
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
int rm(Arvore* raiz, char* caminho) {
    if (raiz == NULL || *raiz == NULL || caminho == NULL)
        return 0;


    NO* atual = (*raiz)->filho;
    NO* anterior = NULL;


    while (atual != NULL) {
        if (atual->nome && strcmp(atual->nome, caminho) == 0) {
            if (anterior == NULL) {
                (*raiz)->filho = atual->irmao;
            } else {
                anterior->irmao = atual->irmao;
            }

            atual->irmao = NULL;

            removeRec(atual);
            return 1; 
        }
        anterior = atual;
        atual = atual->irmao;
    }

    return 0; 
}

/*
    Lista todos os componentes dentro da pasta atual.
*/
int list(Arvore* raiz) {
    if(raiz == NULL || *raiz == NULL) {
        printf("a");
        return 0;
    }
    
    char* caminho = (*raiz)->nome ? (*raiz)->nome : "raiz"; 

    NO* atual = *raiz;

    // Exibe o caminho atual e a lista de arquivos

    printf("\n%s/\n", caminho);
    NO* filho = atual->filho;
    if (filho == NULL) {
        printf("caminho vazio.\n");
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

    // Cria a nova pasta
    NO *no = (NO*) malloc(sizeof(NO));
    if (no == NULL)
        return 0;

    no->caminho = strdup(caminhoCompleto);
    no->nome = strdup(nome);
    no->extensao = NULL; 
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
    no->extensao = strdup(extensao); 
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
    printf(" - cd <caminho> - Muda para o caminho especificado.\n");
    printf(" - list - Lista os arquivos e pastas no caminho atual.\n");
    printf(" - listAll - Lista todos os arquivos e pastas na arvore a partir do caminho atual.\n");
    printf(" - mkdir <nome> - Cria um novo caminho.\n");
    printf(" - rm <nome> - remove  um  pasta  e  seus  arquivos,  deve  fazer  uma  liberacao recursiva.\n");
    printf(" - search <nome> - busca  um  arquivo  ou  pasta  pelo  seu  nome.\n");
    printf(" - clear - Limpa a tela do terminal.\n");
    printf(" - help - Exibe este menu de ajuda.\n");
    printf(" - echo <mensagem> - Exibe a mensagem especificada.\n");
    printf(" - rename <nome_antigo> \"<nome_novo>\" - Renomeia um arquivo ou caminho.\n");
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

    // loop do terminal, só sai com exit
    int sair = 0;
    while (sair == 0) {
        // Prompt
        if (atual != NULL && atual->caminho != NULL) {
            printf("[%s] $ ", atual->caminho);
        } else {
            printf("[raiz] $ ");
        }

        if (!fgets(linha, sizeof(linha), stdin)) {
            printf("\n");
            continue;
        }

        // Remove '\n'
        linha[strcspn(linha, "\n")] = '\0';

        // Recebe comando e arg separados em tokens
        comando = strtok(linha, " \t");
        arg = strtok(NULL, "\t");
        

        if (comando == NULL) {
            continue;
        }

        // Comandos

        if (strcmp(comando, "cd") == 0) {
            if (arg == NULL) {
                printf("Uso: cd <caminho>\n");
            } else {
                atual = cd(atual, arg);
            }
        }
        else if (strcmp(comando, "list") == 0) {
            list(&atual);    
        }
        else if (strcmp(comando, "listAll") == 0) {
            listAll(&atual, arg);    
        }
        else if (strcmp(comando, "mkdir") == 0) {
            if (arg == NULL) {
                printf("Uso: mkdir <nome>\n");
            } else {
                char caminho[QTD_CARACTERES_LEITURA];
                sprintf(caminho, "%s/%s", atual->caminho, arg);
                mkdir_(&atual, caminho);
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
                if(search(raiz, arg) != NULL) {
                    printf("Arquivo ou pasta encontrado: %s\n", search(raiz, arg));
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
                printf("Uso: rename <nome_antigo> \"<nome_novo>\" \n");
            } else {
                if (arg == NULL) {
                    printf("Uso: rename <nome_antigo> \"<nome_novo>\" \n");
                } else {
                    rename_nome(&atual, arg);
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


int rename_nome(Arvore* raiz, char* arg) {
    if (raiz == NULL || *raiz == NULL || arg == NULL) {
        printf("aaaa");
        printf("Uso: rename <nome_antigo> \"<nome_novo>\"\n");
        return 0;
    }
    char *aspa1 = strchr(arg, '"'); // Encontra a primeira aspa
    if (aspa1 == NULL) {
        printf("Uso: rename <nome_antigo> \"<nome_novo>\"\n");
        return 0;
    }

    *aspa1 = '\0'; // Finaliza o buffer do nome antigo

    char *nome_antigo = arg; 
    int tamanho_nome_antigo = strlen(nome_antigo);

    while (tamanho_nome_antigo > 0 && nome_antigo[tamanho_nome_antigo - 1] == ' ') {
        nome_antigo[tamanho_nome_antigo-1]= '\0'; // Remove espaços no final
    }

    char *aspa2 = strchr(aspa1 + 1, '"'); // Encontra a segunda aspa
    if (aspa2 == NULL) {
        printf("Uso: rename <nome_antigo> \"<nome_novo>\"\n");
        return 0;
    }
    *aspa2 = '\0'; // Finaliza o buffer do novo nome
    char *nome_novo = aspa1 + 1; 
    
    
    
    char* caminho = search(raiz, nome_antigo);
    int j =strlen(caminho);
    while(caminho[j] != '/' || j == 0) {
        caminho[j] = '\0'; 
        j--;
    }
    caminho[j] = '\0'; 

    // Percorre filhos até encontrar o nome antigo e altera o nome e caminho
    NO* atual = (*raiz)->filho; 
    if (atual == NULL) {
        printf("Arvore vazia.\n");
        return 0;
    }
    while (atual != NULL) {
        if (atual->nome != NULL && strcmp(atual->nome, nome_antigo) == 0) {
            free(atual->nome);

            atual->nome = strdup(nome_novo);
            
            atual->caminho = realloc(atual->caminho, strlen(caminho) + strlen(nome_novo) + 2);

            if (atual->caminho == NULL) {
                printf("Erro ao alocar memoria para o caminho.\n");
                return 0;
            }

            sprintf(atual->caminho, "%s/%s", caminho, nome_novo);
            printf("Renomeado '%s' para '%s'.\n", nome_antigo, nome_novo);

            return 1;
        }
        atual = atual->irmao;
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

int listAll(Arvore* raiz, char* caminho) {
    if (raiz == NULL || *raiz == NULL || caminho == NULL) {
        return 0;
    }

    NO* atual = *raiz;

    while (atual != NULL) {

        if (strcmp(atual->nome, caminho) == 0) {
            NO* comp = atual->filho;

            if (comp == NULL) {
                printf("Diretorio '%s' esta vazio.\n", caminho);
                return 1;
            }

            printf("\nConteudo da pasta '%s':\n", caminho);
            while (comp != NULL) {
                printf("  - %s\n", comp->nome);
                comp = comp->irmao;
            }
            return 1;
        }

        if (atual->filho != NULL) {
            atual = atual->filho;

        } else if (atual->irmao != NULL) {
            atual = atual->irmao;

        } else {

            while (atual != NULL && atual->irmao == NULL) {
                atual = atual->pai;
            }
            if (atual != NULL) {
                atual = atual->irmao;
            }
        }
    }

    printf("Diretorio '%s' nao encontrado na arvore.\n", caminho);
    return 0;
}

