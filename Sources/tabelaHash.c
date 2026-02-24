#include "tabelaHash.h"
#include "produto.h"
#include <stdlib.h>
#include <stdio.h>

// Definicoes de status para os slots
const int ID_VAZIO = -1;
const int ID_REMOVIDO = -2;

// Funcao hash que embaralha melhor os bits da chave para uma distribuicao mais aleatoria.
int funcao_hash(int chave, int M) {
    unsigned int c = chave;
    c = (c >> 16 ^ c) * 0x45d9f3b;
    c = (c >> 16 ^ c) * 0x45d9f3b;
    c = c >> 16 ^ c;
    return c % M;
}

void inicializar_tabela_hash(FILE *arq, int M) {
    printf("Inicializando Tabela Hash com %d posicoes...\n", M);
    TProduto *vazio = produto(ID_VAZIO, "", 0.0, 0, 0);

    rewind(arq);
    for (int i = 0; i < M; i++) {
        salva_produto(vazio, arq);
    }

    free(vazio);
    printf("Tabela inicializada!\n");
}

int inserir_hash(FILE *arq, TProduto *p, int M, int pos_inicial) {
    int pos = pos_inicial;

    int tam_reg = tamanho_registro_produto();

    for (int i = 0; i < M; i++) {
        fseek(arq, pos * tam_reg, SEEK_SET);
        TProduto *lido = le_produto(arq);

        if (lido == NULL) {
            return -1;
        }

        //se encontrou um espaco vazio ou removido
        if (lido->id_produto == ID_VAZIO || lido->id_produto == ID_REMOVIDO) {
            fseek(arq, pos * tam_reg, SEEK_SET);
            salva_produto(p, arq);
            free(lido);
            return pos;
        }

        free(lido);

        //se o espaco esta ocupado, vai pro proximo
        pos = (pos + 1) % M;
    }

    printf("Erro: Tabela Hash cheia! Nao foi possivel inserir ID %d.\n", p->id_produto);
    return -1;
}

TProduto* busca_hash(FILE *arq, int chave, int M) {
    int pos = funcao_hash(chave, M);
    int pos_inicial = pos;
    int tam_reg = tamanho_registro_produto();

    while (1) {
        fseek(arq, pos * tam_reg, SEEK_SET);
        TProduto *lido = le_produto(arq);

        if (lido == NULL) return NULL;

        if (lido->id_produto == chave && lido->id_produto != ID_VAZIO && lido->id_produto != ID_REMOVIDO) {
            return lido;
        }

        free(lido);

        pos = (pos + 1) % M;

        if (pos == pos_inicial) {
            return NULL;
        }
    }
}

void remover_hash(FILE *arq, int chave, int M) {
    int pos = funcao_hash(chave, M);
    int pos_inicial = pos;
    int tam_reg = tamanho_registro_produto();

    while (1) {
        fseek(arq, pos * tam_reg, SEEK_SET);
        TProduto *lido = le_produto(arq);

        if (lido == NULL) return;

        if (lido->id_produto == chave) {

            lido->id_produto = ID_REMOVIDO;

            fseek(arq, pos * tam_reg, SEEK_SET);
            salva_produto(lido, arq);

            printf("Produto ID %d removido da posicao %d (Espaco disponivel para reuso).\n", chave, pos);
            free(lido);
            return;
        }

        free(lido);
        pos = (pos + 1) % M;

        if (pos == pos_inicial) {
            printf("Erro: ID %d nao encontrado.\n", chave);
            return;
        }
    }
}

void imprimir_tabela_hash(FILE *arq, int M) {
    rewind(arq);
    int tam_reg = tamanho_registro_produto();
    TProduto *p;

    printf("\n--- ESTADO DA TABELA HASH ---\n");
    for (int i = 0; i < M; i++) {
        fseek(arq, i * tam_reg, SEEK_SET);
        p = le_produto(arq);

        if (p != NULL) {
            if (p->id_produto == ID_VAZIO) {
                printf("[%d]: VAZIO\n", i);
            } else if (p->id_produto == ID_REMOVIDO) {
                printf("[%d]: REMOVIDO (Disponivel)\n", i);
            } else {
                printf("[%d]: ID %d | %s\n", i, p->id_produto, p->nome_produto);
            }
            free(p);
        }
    }
    printf("-----------------------------\n");
}