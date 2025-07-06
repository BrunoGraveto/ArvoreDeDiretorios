# Trabalho 2 – Árvores  
**Disciplina:** Algoritmos e Estrutura de Dados III  
**Professor:** Thiago Naves  
**Instituição:** Universidade Tecnológica Federal do Paraná – Campus Santa Helena  

## 🎯 Objetivo

Este projeto tem como objetivo implementar, em linguagem C, uma árvore genérica de diretórios capaz de simular uma linha de comando básica. A árvore representa a estrutura de arquivos e diretórios informados via um arquivo de entrada, e permite navegação e manipulação através de comandos do tipo shell.

## 📁 Estrutura do Projeto

- `matriz.h` – Declaração das estruturas e funções utilizadas.  
- `matriz.c` – Implementação das funções definidas.  
- `main.c` – Função principal com interação com o usuário via linha de comando.  
- `in.txt` – Arquivo de entrada contendo a estrutura de diretórios a ser carregada na árvore.  
- `documento.pdf` – Relatório explicando o funcionamento das funções, decisões de projeto e desafios enfrentados.

## 🧩 Funcionalidades

O programa deve ser capaz de:

- 📂 **Ler** um arquivo `in.txt` com uma lista de caminhos de diretórios e arquivos.
- 📁 **Montar uma árvore** com base nesses caminhos.
- 🧭 **Navegar** entre os diretórios com comandos via terminal.

## 🧪 Comandos Disponíveis

| Comando          | Descrição |
|------------------|-----------|
| `cd <dir>`       | Entra no diretório especificado. Se não existir, sugere alternativas semelhantes. |
| `search <nome>`  | Busca por um arquivo ou pasta e retorna o caminho completo. |
| `rm <dir>`       | Remove um diretório e todos os seus conteúdos de forma recursiva. |
| `list`           | Lista todos os itens do diretório atual. |
| `mkdir <nome>`   | Cria um novo diretório no diretório atual. |
| `clear`          | Limpa o terminal (simulado com múltiplos `printf` ou comando `clear/cls`). |
| `help`           | Exibe a lista de comandos e suas descrições. |
| `exit`           | Encerra o programa liberando a memória alocada. |

## 📄 Documentação

Um documento anexo ao repositório descreve os requisios, funcionamento interno do código, estratégias utilizadas na implementação e dificuldades encontradas no desenvolvimento do projeto.

📎 Acesse a documentação completa em:  
[**Relatório Técnico - PDF**](./docs/Documentação.pdf)

---

**Grupo:**  
- Bruno Eduardo Santos Alcantara, RA: 2677156
- Caio Macedo Lima da Cruz, RA: 2651378
- Jose Carlos Seben de Souza Leite, RA: 2651130
