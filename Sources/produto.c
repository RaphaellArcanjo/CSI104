#include "produto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Cria fornecedor.
TProduto *produto(int id_produto, char *nome_produto, double preco, int estoque, int id_fornecedor) {
    TProduto *produto = (TProduto *) malloc(sizeof(TProduto));
    //inicializa espaco de memoria com ZEROS
    if (produto) {
        memset(produto, 0, sizeof(TProduto));
    }
    //copia valores para os campos de func
    produto->id_produto = id_produto;
    strcpy(produto->nome_produto, nome_produto);
    produto->preco = preco;
    produto->estoque = estoque;
    produto->id_fornecedor = id_fornecedor;
    return produto;
}

// Salva produto no arquivo out, na posicao atual do cursor
void salva_produto(TProduto *produto, FILE *out) {
    fwrite(&produto->id_produto, sizeof(int), 1, out);
    //produto->nome_produto ao inves de &produto->nome_produto, pois string ja eh um ponteiro
    fwrite(produto->nome_produto, sizeof(char), sizeof(produto->nome_produto), out);
    fwrite(&produto->preco, sizeof(double), 1, out);
    fwrite(&produto->estoque, sizeof(int), 1, out);
    fwrite(&produto->id_fornecedor, sizeof(int), 1, out);
}

// Le um produto do arquivo in na posicao atual do cursor
// Retorna um ponteiro para produto lido do arquivo
TProduto *le_produto(FILE *in) {
    TProduto *produto = (TProduto *) malloc(sizeof(TProduto));
    if (0 >= fread(&produto->id_produto, sizeof(int), 1, in)) {
        free(produto);
        return NULL;
    }
    fread(produto->nome_produto, sizeof(char), sizeof(produto->nome_produto), in);
    fread(&produto->preco, sizeof(double), 1, in);
    fread(&produto->estoque, sizeof(int), 1, in);
    fread(&produto->id_fornecedor, sizeof(int), 1, in);
    return produto;
}

// Imprime produto
void imprime_produto(TProduto *produto) {
    printf("Codigo do produto: %d\n", produto->id_produto);
    printf("Nome do produto: %s\n", produto->nome_produto);
    printf("Preco: %.2lf\n", produto->preco);
    printf("Estoque: %d\n", produto->estoque);
    printf("ID fornecedor: %d\n", produto->id_fornecedor);
    printf("**********************************************\n");
}


// Retorna tamanho do produto em bytes
int tamanho_registro_produto() {
    return sizeof(int)  //id_produto
           + sizeof(char) * 50 //nome_produto
           + sizeof(double) //preco
           + sizeof(int) //estoque
           + sizeof(int); //id_fornecedor;
}

// Cria a base de dados
void criar_base_produto(FILE *out, int tam, int num_fornecedores){

    int *vet = (int*) malloc(tam * sizeof(int));
    if (vet == NULL) {
        printf("\nERRO: Falha ao alocar memoria para o vetor de indices.\n");
        return;
    }

    TProduto *p;

    for(int i=0;i<tam;i++) {
        vet[i] = i+1;
    }

    embaralha_produto(vet,tam);

    printf("\nGerando a base de dados de produtos...\n");

    if (num_fornecedores <= 0) {
        printf("AVISO: Nao ha fornecedores para linkar. Os produtos serao criados com ID Fornecedor 0.\n");
    }

    for (int i=0;i<tam;i++){

        double preco_aleatorio = ((double)(rand() % 99999) + 1) / 100.0;

        int estoque_aleatorio = rand() % 50 + 1;

        int id_forn_aleatorio = 0;

        if (num_fornecedores > 0) {
            id_forn_aleatorio = rand() % num_fornecedores + 1;
        }

        p = produto(vet[i], "NOME DO PRODUTO", preco_aleatorio, estoque_aleatorio, id_forn_aleatorio);

        salva_produto(p, out);
    }

    free(p);
    free(vet);
}

//embaralha base de dados
void embaralha_produto(int *vet, int tam) {

    int tmp;

    srand(time(NULL));

    int trocas = (tam*60)/100;

    for (int t = 1; t<trocas; t++) {
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

void imprimir_base_produto(FILE *out){

    printf("\Imprimindo a base de dados...\n");
    printf("**********************************************\n");
    rewind(out);
    TProduto *p;

    while ((p = le_produto(out)) != NULL)
        imprime_produto(p);

    free(p);
}

int tamanho_arquivo_produto(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro_produto());
    return tam;
}
