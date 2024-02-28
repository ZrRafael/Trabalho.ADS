#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Estrutura para representar um livro
struct Livro {
    int id;
    char titulo[100];
    char autor[100];
    struct Livro* proximo; // Utilizado para lidar com colisões por encadeamento
};

// Tamanho da tabela de dispersão
#define TAMANHO_TABELA 100

// Função de hash simples para mapear IDs para índices da tabela
int hash(int id) {
    return id % TAMANHO_TABELA;
}

// Função para inicializar a tabela de dispersão
struct Livro** inicializarTabela() {
    struct Livro** tabela = (struct Livro**)malloc(TAMANHO_TABELA * sizeof(struct Livro*));
    if (tabela == NULL) {
        perror("Erro ao alocar memória para a tabela de dispersão");
        exit(EXIT_FAILURE);
    }

    // Inicializa todas as entradas da tabela como NULL
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela[i] = NULL;
    }
    return tabela;
}

// Função para adicionar um livro à tabela de dispersão
void adicionarLivro(struct Livro** tabela, int id, const char* titulo, const char* autor) {
    int indice = hash(id);

    // Alocando memória para um novo livro
    struct Livro* novoLivro = (struct Livro*)malloc(sizeof(struct Livro));
    if (novoLivro == NULL) {
        perror("Erro ao alocar memória para um novo livro");
        exit(EXIT_FAILURE);
    }

    // Preenchendo os dados do novo livro
    novoLivro->id = id;
    strcpy(novoLivro->titulo, titulo);
    strcpy(novoLivro->autor, autor);
    novoLivro->proximo = NULL;

    // Lidando com colisões utilizando encadeamento
    if (tabela[indice] == NULL) {
        tabela[indice] = novoLivro;
    } else {
        struct Livro* atual = tabela[indice];
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoLivro;
    }
}

// Função para buscar um livro por ID na tabela de dispersão
struct Livro* buscarLivroPorID(struct Livro** tabela, int id) {
    int indice = hash(id);

    // Percorrendo a lista encadeada no índice correspondente
    struct Livro* atual = tabela[indice];
    while (atual != NULL) {
        // Verificando se o livro com o ID correto foi encontrado
        if (atual->id == id) {
            return atual; // Livro encontrado
        }
        atual = atual->proximo;
    }

    return NULL; // Livro não encontrado
}

// Função para liberar a memória alocada para a tabela de dispersão
void liberarMemoria(struct Livro** tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        struct Livro* atual = tabela[i];
        while (atual != NULL) {
            struct Livro* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
    free(tabela);
}

int main() {
    // Configurando o locale para imprimir caracteres especiais (acentos, etc.)
    setlocale(LC_ALL, "portuguese");

    // Inicializando a tabela de dispersão
    struct Livro** tabela = inicializarTabela();

    // Adicionando livros à tabela
    adicionarLivro(tabela, 1, "Berserk", "Kentaro Miura");
    adicionarLivro(tabela, 2, "Drácula", "Bram Stoker");
    adicionarLivro(tabela, 3, "Diário de um Banana", "Jeff Kinney");

    // Solicitando ao usuário o número de identificação do livro desejado
    int idBusca;
    printf("Digite o número de identificação do livro (1) (2) (3): ");
    scanf("%d", &idBusca);

    // Busca por livro pelo número de identificação
    struct Livro* livroEncontrado = buscarLivroPorID(tabela, idBusca);

    // Exibindo resultado da busca
    if (livroEncontrado != NULL) {
        printf("Livro encontrado - Título: %s, Autor: %s\n", livroEncontrado->titulo, livroEncontrado->autor);
    } else {
        printf("Livro com ID %d não encontrado na biblioteca.\n", idBusca);
    }

    // Liberando memória alocada
    liberarMemoria(tabela);

    return 0;
}
