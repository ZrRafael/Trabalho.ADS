#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Estrutura para representar um livro
struct Livro {
    int id;
    char titulo[100];
    char autor[100];
    struct Livro* proximo; // Utilizado para lidar com colis�es por encadeamento
};

// Tamanho da tabela de dispers�o
#define TAMANHO_TABELA 100

// Fun��o de hash simples para mapear IDs para �ndices da tabela
int hash(int id) {
    return id % TAMANHO_TABELA;
}

// Fun��o para inicializar a tabela de dispers�o
struct Livro** inicializarTabela() {
    struct Livro** tabela = (struct Livro**)malloc(TAMANHO_TABELA * sizeof(struct Livro*));
    if (tabela == NULL) {
        perror("Erro ao alocar mem�ria para a tabela de dispers�o");
        exit(EXIT_FAILURE);
    }

    // Inicializa todas as entradas da tabela como NULL
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela[i] = NULL;
    }
    return tabela;
}

// Fun��o para adicionar um livro � tabela de dispers�o
void adicionarLivro(struct Livro** tabela, int id, const char* titulo, const char* autor) {
    int indice = hash(id);

    // Alocando mem�ria para um novo livro
    struct Livro* novoLivro = (struct Livro*)malloc(sizeof(struct Livro));
    if (novoLivro == NULL) {
        perror("Erro ao alocar mem�ria para um novo livro");
        exit(EXIT_FAILURE);
    }

    // Preenchendo os dados do novo livro
    novoLivro->id = id;
    strcpy(novoLivro->titulo, titulo);
    strcpy(novoLivro->autor, autor);
    novoLivro->proximo = NULL;

    // Lidando com colis�es utilizando encadeamento
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

// Fun��o para buscar um livro por ID na tabela de dispers�o
struct Livro* buscarLivroPorID(struct Livro** tabela, int id) {
    int indice = hash(id);

    // Percorrendo a lista encadeada no �ndice correspondente
    struct Livro* atual = tabela[indice];
    while (atual != NULL) {
        // Verificando se o livro com o ID correto foi encontrado
        if (atual->id == id) {
            return atual; // Livro encontrado
        }
        atual = atual->proximo;
    }

    return NULL; // Livro n�o encontrado
}

// Fun��o para liberar a mem�ria alocada para a tabela de dispers�o
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

    // Inicializando a tabela de dispers�o
    struct Livro** tabela = inicializarTabela();

    // Adicionando livros � tabela
    adicionarLivro(tabela, 1, "Berserk", "Kentaro Miura");
    adicionarLivro(tabela, 2, "Dr�cula", "Bram Stoker");
    adicionarLivro(tabela, 3, "Di�rio de um Banana", "Jeff Kinney");

    // Solicitando ao usu�rio o n�mero de identifica��o do livro desejado
    int idBusca;
    printf("Digite o n�mero de identifica��o do livro (1) (2) (3): ");
    scanf("%d", &idBusca);

    // Busca por livro pelo n�mero de identifica��o
    struct Livro* livroEncontrado = buscarLivroPorID(tabela, idBusca);

    // Exibindo resultado da busca
    if (livroEncontrado != NULL) {
        printf("Livro encontrado - T�tulo: %s, Autor: %s\n", livroEncontrado->titulo, livroEncontrado->autor);
    } else {
        printf("Livro com ID %d n�o encontrado na biblioteca.\n", idBusca);
    }

    // Liberando mem�ria alocada
    liberarMemoria(tabela);

    return 0;
}
