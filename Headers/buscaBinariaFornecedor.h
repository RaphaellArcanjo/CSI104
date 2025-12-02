#ifndef R_BUSCABINARIAFORNECEDOR_H_INCLUDED
#define R_BUSCABINARIAFORNECEDOR_H_INCLUDED
#include "fornecedor.h"

TFornecedor *busca_binaria_fornecedor(int chave, FILE *in, int inicio, int fim, int *comparacao);

#endif
