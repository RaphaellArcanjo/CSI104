//
// Created by raphaell on 05/02/2026.
//

#ifndef R_TABELAHASH_H
#define R_TABELAHASH_H

#include <stdio.h>
#include "produto.h"

void inicializar_tabela_hash(FILE *arq, int M);

int funcao_hash(int chave, int M);

int inserir_hash(FILE *arq, TProduto *p, int M, int pos_inicial);

TProduto* busca_hash(FILE *arq, int chave, int M);

void remover_hash(FILE *arq, int chave, int M);

void imprimir_tabela_hash(FILE *arq, int M);

#endif