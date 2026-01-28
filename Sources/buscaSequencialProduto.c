#include "produto.h"
#include <stdlib.h>

//Realiza uma busca sequencial por um produto na base de dados
TProduto *busca_sequencial_produto(int chave, FILE *in, int *comparacao){

    TProduto *p = NULL;
    *comparacao = 0;

    rewind(in);

    while ((p = le_produto(in)) != NULL){
        *comparacao += 1;
        if(p->id_produto == chave){
           return p;
        }
        free(p);
    }
    return NULL;
}
