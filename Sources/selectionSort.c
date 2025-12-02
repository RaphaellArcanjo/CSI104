#include "fornecedor.h"
#include "produto.h"
#include <stdlib.h>
#include <stdio.h>

int selection_sort_disco_fornecedor(FILE *in) {

    int n = tamanho_arquivo_fornecedor(in);
    if (n <= 1) {
        return 0;
    }

    //pega o tamanho em bytes de cada registro
    int tam_registro = tamanho_registro_fornecedor();
    int comparacoes = 0;


    for (int i = 0; i < n - 1; i++) {

        int pos_min = i;

        //minimo temporário para comparações
        fseek(in, i * tam_registro, SEEK_SET);
        TFornecedor *reg_min = le_fornecedor(in);

        //procura o menor elemento de 'i+1' até o fim
        for (int j = i + 1; j < n; j++) {

            fseek(in, j * tam_registro, SEEK_SET);
            TFornecedor *reg_j = le_fornecedor(in);

            comparacoes++;

            if (reg_j->id_fornecedor < reg_min->id_fornecedor) {
                free(reg_min);
                reg_min = reg_j;
                pos_min = j;
            } else {
                free(reg_j);
            }
        }

        if (pos_min != i) {

            fseek(in, i * tam_registro, SEEK_SET);
            TFornecedor *reg_i = le_fornecedor(in);

            fseek(in, i * tam_registro, SEEK_SET);
            salva_fornecedor(reg_min, in); //

            fseek(in, pos_min * tam_registro, SEEK_SET);
            salva_fornecedor(reg_i, in); //

            free(reg_i);
        }

        free(reg_min);
    }

    rewind(in);

    return comparacoes;
}

int selection_sort_disco_produto(FILE *in) {

    int n = tamanho_arquivo_produto(in);
    if (n <= 1) {
        return 0;
    }

    int tam_registro = tamanho_registro_produto();
    int comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        int pos_min = i;

        fseek(in, i * tam_registro, SEEK_SET);
        TProduto *reg_min = le_produto(in);

        for (int j = i + 1; j < n; j++) {
            fseek(in, j * tam_registro, SEEK_SET);
            TProduto *reg_j = le_produto(in);

            comparacoes++;


            if (reg_j->id_produto < reg_min->id_produto) {
                free(reg_min);
                reg_min = reg_j;
                pos_min = j;
            } else {
                free(reg_j);
            }
        }

        if (pos_min != i) {
            fseek(in, i * tam_registro, SEEK_SET);
            TProduto *reg_i = le_produto(in);

            fseek(in, i * tam_registro, SEEK_SET);
            salva_produto(reg_min, in);

            fseek(in, pos_min * tam_registro, SEEK_SET);
            salva_produto(reg_i, in);

            free(reg_i);
        }

        free(reg_min);
    }

    rewind(in);

    return comparacoes;
}