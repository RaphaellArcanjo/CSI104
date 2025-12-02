#ifndef R_PRODUTO_H
#define R_PRODUTO_H
#include <stdio.h>

typedef struct Produto {
    int id_produto;
    char nome_produto[50];
    double preco;
    int estoque;
    int id_fornecedor; //interacao
} TProduto;


// Cria produto.
TProduto *produto(int id_produto, char *nome_produto, double preco, int estoque, int id_fornecedor);


// Salva produto no arquivo out, na posicao atual do cursor
void salva_produto(TProduto *produto, FILE *out);


// Le um produto do arquivo in na posicao atual do cursor
// Retorna um ponteiro para produto lido do arquivo
TProduto *le_produto(FILE *in);


// Imprime produto
void imprime_produto(TProduto *produto);

// Retorna tamanho do produto em bytes
int tamanho_registro_produto();

// Cria a base de dados
void criar_base_produto(FILE *out, int tam, int num_fornecedores);

//embaralha base de dados
void embaralha_produto(int *vet, int tam);

// Imprime a base de dados
void imprimir_base_produto(FILE *out);

//Retorna a quantidade de registros no arquivo
int tamanho_arquivo_produto(FILE *arq);

#endif