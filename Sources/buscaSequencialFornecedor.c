#include "fornecedor.h"
#include <string.h>
#include <stdlib.h>

//Realiza uma busca sequencial por um fornecedor na base de dados
TFornecedor *busca_sequencial_fornecedor(int chave, FILE *in, int *comparacao){

    TFornecedor *f = NULL;
    *comparacao = 0;

    rewind(in);

    while ((f = le_fornecedor(in)) != NULL){
        *comparacao += 1;
        if(f->id_fornecedor == chave){
           return f;
        }
    }
    return NULL;
}
