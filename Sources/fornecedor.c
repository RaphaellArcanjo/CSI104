#include "fornecedor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Cria fornecedor.
TFornecedor *fornecedor(int id_fornecedor, char *razao_social, char *cnpj) {
    TFornecedor *fornecedor = (TFornecedor *) malloc(sizeof(TFornecedor));
    //inicializa espaco de memoria com ZEROS
    if (fornecedor) {
        memset(fornecedor, 0, sizeof(TFornecedor));
    }
    //copia valores para os campos de func
    fornecedor->id_fornecedor = id_fornecedor;
    strcpy(fornecedor->razao_social, razao_social);
    strcpy(fornecedor->cnpj, cnpj);
    return fornecedor;
}

// Salva fornecedor no arquivo out, na posicao atual do cursor
void salva_fornecedor(TFornecedor *fornecedor, FILE *out) {
    fwrite(&fornecedor->id_fornecedor, sizeof(int), 1, out);
    //fornecedor->razao_social ao inves de &fornecedor->razao_social, pois string ja eh um ponteiro
    fwrite(fornecedor->razao_social, sizeof(char), sizeof(fornecedor->razao_social), out);
    fwrite(fornecedor->cnpj, sizeof(char), sizeof(fornecedor->cnpj), out);
}

// Le um fornecedor do arquivo in na posicao atual do cursor
// Retorna um ponteiro para fornecedor lido do arquivo
TFornecedor *le_fornecedor(FILE *in) {
    TFornecedor *fornecedor = (TFornecedor *) malloc(sizeof(TFornecedor));
    if (0 >= fread(&fornecedor->id_fornecedor, sizeof(int), 1, in)) {
        free(fornecedor);
        return NULL;
    }
    fread(fornecedor->razao_social, sizeof(char), sizeof(fornecedor->razao_social), in);
    fread(fornecedor->cnpj, sizeof(char), sizeof(fornecedor->cnpj), in);
    return fornecedor;
}

// Imprime fornecedor
void imprime_fornecedor(TFornecedor *fornecedor) {
    printf("Codigo do fornecedor: %d\n", fornecedor->id_fornecedor);
    printf("Razao social: %s\n", fornecedor->razao_social);
    printf("CNPJ: %s\n", fornecedor->cnpj);
    printf("**********************************************\n");
}


// Retorna tamanho do fornecedor em bytes
int tamanho_registro_fornecedor() {
    return sizeof(int)  //id_fornecedor
           + sizeof(char) * 50 //razao_social
           + sizeof(char) * 19; //cnpj
}

// Cria a base de dados
void criar_base_fornecedor(FILE *out, int tam){

    int *vet = (int*) malloc(tam*sizeof(int));
    if (vet == NULL) {
        printf("\nERRO: Falha ao alocar memoria para o vetor de indices.\n");
        return;
    }

    TFornecedor *f;

    for(int i=0;i<tam;i++) {
        vet[i] = i+1;
    }

    embaralha_fornecedor(vet,tam);

    printf("Gerando a base de dados...\n");

    for (int i=0;i<tam;i++){
        f = fornecedor(vet[i], "NOME DA EMPRESA", "XX.XXX.XXX/YYYY-ZZ");
        salva_fornecedor(f, out);
    }

    free(f);
    free(vet);
}

//embaralha base de dados
void embaralha_fornecedor(int *vet, int tam) {

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

void imprimir_base_fornecedor(FILE *out){

    printf("Imprimindo a base de dados...\n");
    printf("**********************************************\n");

    rewind(out);
    TFornecedor *f;

    while ((f = le_fornecedor(out)) != NULL)
        imprime_fornecedor(f);

    free(f);
}

int tamanho_arquivo_fornecedor(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro_fornecedor());
    return tam;
}
