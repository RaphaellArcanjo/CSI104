#ifndef R_INTERFACE_H
#define R_INTERFACE_H

#define ARQUIVO_FORNECEDORES "fornecedores.dat"
#define ARQUIVO_PRODUTOS "produtos.dat"

#include <stdio.h>

void MSG_MENU();

void limpa_tela();

void limpa_buffer_stdin();

void pausa();

// op_gerar_base precisa de ponteiro-para-ponteiro (**); para poder fechar e reabrir os arquivos do main.
void op_gerar_base(FILE **arq_f, FILE **arq_p);

void op_imprimir_bases(FILE *arq_f, FILE *arq_p);

void op_ordenar_bases(FILE *arq_f, FILE *arq_p);

void op_busca_sequencial(FILE *arq_f, FILE *arq_p);

void op_busca_binaria(FILE *arq_f, FILE *arq_p);

void op_listar_produtos_por_fornecedor(FILE *arq_f, FILE *arq_p);

void op_atualizar_preco_produto(FILE *arq_p);

//PARTE 2
void op_gerar_particoes(FILE *arq_p);

void op_intercalacao_otima(FILE *arq_p);

void op_analise_experimental(FILE *arq_p);

#endif