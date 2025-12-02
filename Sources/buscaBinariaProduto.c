#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "produto.h"
#include "buscaBinariaProduto.h"

TProduto *busca_binaria_produto(int chave, FILE *in, int inicio, int fim, int *comparacao) {

    TProduto *p = NULL;
    int cod = -1;
    *comparacao = 0;

    int tam_reg = tamanho_registro_produto();

    while (inicio <= fim) {

        int meio = (inicio + fim) / 2;

        fseek(in, meio * tam_reg, SEEK_SET);
        p = le_produto(in);

        if (p == NULL) {
            break;
        }

        cod = p->id_produto;
        *comparacao += 1;

        if (cod == chave) {
            return p;
        }

        if (cod > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }

        free(p);
    }

    return NULL;
}