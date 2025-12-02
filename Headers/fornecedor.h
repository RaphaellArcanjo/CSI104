#ifndef R_FUNCIONARIO_H_INCLUDED
#define R_FUNCIONARIO_H_INCLUDED
#include <stdio.h>

typedef struct Fornecedor {
    int id_fornecedor;
    char razao_social[50];
    char cnpj[19]; //XX.XXX.XXX/YYYY-ZZ
} TFornecedor;


// Cria fornecedor.
TFornecedor *fornecedor(int id_fornecedor, char *razao_social, char *cnpj);


// Salva funcionario no arquivo out, na posicao atual do cursor
void salva_fornecedor(TFornecedor *fornecedor, FILE *out);


// Le um fornecedor do arquivo in na posicao atual do cursor
// Retorna um ponteiro para fornecedor lido do arquivo
TFornecedor *le_fornecedor(FILE *in);


// Imprime fornecedor
void imprime_fornecedor(TFornecedor *fornecedor);

// Retorna tamanho do fornecedor em bytes
int tamanho_registro_fornecedor();

// Cria a base de dados
void criar_base_fornecedor(FILE *out, int tam);

//embaralha base de dados
void embaralha_fornecedor(int *vet, int tam);

// Imprime a base de dados
void imprimir_base_fornecedor(FILE *out);

//Retorna a quantidade de registros no arquivo
int tamanho_arquivo_fornecedor(FILE *arq);



#endif
