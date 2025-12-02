#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "buscaBinariaFornecedor.h"

//Realiza uma busca binaria por um fornecedor na base de dados
TFornecedor *busca_binaria_fornecedor(int chave, FILE *in, int inicio, int fim, int *comparacao) {

    TFornecedor *f = NULL;
    int cod = -1;
    *comparacao = 0;

    // Pega o tamanho do registro uma vez
    int tam_reg = tamanho_registro_fornecedor();

    while (inicio <= fim) {

        int meio = (inicio + fim) / 2;

        fseek(in, meio * tam_reg, SEEK_SET);
        f = le_fornecedor(in);

        if (f == NULL) {
            break;
        }

        cod = f->id_fornecedor;
        *comparacao += 1;

        if (cod == chave) {
            return f;
        }

        if (cod > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }

        free(f);
    }

    return NULL;
}