#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> //INT_MAX

#include "particoes.h"
#include "produto.h"

typedef struct {
    TProduto *produto;
    int congelado;
} TElementoHeap;

void trocar(TElementoHeap *a, TElementoHeap *b) {
    TElementoHeap temp = *a;
    *a = *b;
    *b = temp;
}

void refaz_heap(TElementoHeap *heap, int i, int tam_heap) {
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    int menor = i;

    if (esq < tam_heap && heap[esq].produto->id_produto < heap[menor].produto->id_produto) {
        menor = esq;
    }

    if (dir < tam_heap && heap[dir].produto->id_produto < heap[menor].produto->id_produto) {
        menor = dir;
    }

    if (menor != i) {
        trocar(&heap[i], &heap[menor]);
        refaz_heap(heap, menor, tam_heap);
    }
}

void constroi_heap(TElementoHeap *heap, int tam_heap) {
    for (int i = tam_heap / 2 - 1; i >= 0; i--) {
        refaz_heap(heap, i, tam_heap);
    }
}

void selecao_substituicao(FILE *arq_entrada, char *nome_arquivo_saida, int M) {
    printf("\n=== Iniciando Selecao por Substituicao (Memoria: %d) ===\n", M);

    TElementoHeap *heap = (TElementoHeap *) malloc(M * sizeof(TElementoHeap));
    for (int i = 0; i < M; i++) {
        heap[i].produto = (TProduto *) malloc(sizeof(TProduto));
        heap[i].congelado = 0;
    }

    rewind(arq_entrada);

    int n_lidos = 0;
    for (int i = 0; i < M; i++) {
        if (le_produto_no_heap(arq_entrada, heap[i].produto)) {
            n_lidos++;
        } else {
            heap[i].produto->id_produto = INT_MAX;
        }
        heap[i].congelado = 0;
    }

    if (n_lidos == 0) {
        printf("Arquivo de entrada vazio.\n");
        for (int i = 0; i < M; i++) free(heap[i].produto);
        free(heap);
        return;
    }

    constroi_heap(heap, n_lidos);

    int num_particao = 0;
    char nome_particao[100];
    FILE *arq_saida = NULL;
    int tam_atual_heap = n_lidos;
    int ultimo_id_gravado = -1;
    int fim_arquivo_entrada = (n_lidos < M);

    TProduto *prox = (TProduto *) malloc(sizeof(TProduto));
    if (!prox) {
        printf("Erro: Falha ao alocar memoria para 'prox'.\n");
        for (int i = 0; i < M; i++) free(heap[i].produto);
        free(heap);
        return;
    }

    while (1) {
        if (tam_atual_heap == 0) {
            if (arq_saida != NULL) {
                fclose(arq_saida);
                arq_saida = NULL;
            }

            int validos = 0;
            for (int k = 0; k < M; k++) {
                heap[k].congelado = 0; // Descongela
                if (heap[k].produto->id_produto != INT_MAX) {
                    if (k != validos) {
                        trocar(&heap[k], &heap[validos]);
                    }
                    validos++;
                }
            }

            if (validos == 0) {
                break;
            }

            tam_atual_heap = validos;
            constroi_heap(heap, tam_atual_heap);
        }

        if (arq_saida == NULL) {
            num_particao++;
            sprintf(nome_particao, "%s%d.dat", nome_arquivo_saida, num_particao);
            arq_saida = fopen(nome_particao, "w+b");
            if (!arq_saida) {
                perror("Erro ao criar arquivo de particao");
                break;
            }
            ultimo_id_gravado = -1;
        }

        TProduto *min = heap[0].produto;
        salva_produto(min, arq_saida);
        ultimo_id_gravado = min->id_produto;

        if (!fim_arquivo_entrada && le_produto_no_heap(arq_entrada, prox)) {
            if (prox->id_produto < ultimo_id_gravado) {
                *heap[0].produto = *prox;
                heap[0].congelado = 1;
                if (tam_atual_heap > 1) {
                    trocar(&heap[0], &heap[tam_atual_heap - 1]);
                }
                tam_atual_heap--;
            } else {
                *heap[0].produto = *prox;
            }
        } else {
            fim_arquivo_entrada = 1;

            if (tam_atual_heap > 0) {
                 if (tam_atual_heap > 1) {
                    trocar(&heap[0], &heap[tam_atual_heap - 1]);
                }
                tam_atual_heap--;
                heap[tam_atual_heap].produto->id_produto = INT_MAX;
            }
        }

        if (tam_atual_heap > 0) {
            refaz_heap(heap, 0, tam_atual_heap);
        }
    }

    free(prox);
    if (arq_saida) fclose(arq_saida);
    for (int i = 0; i < M; i++) free(heap[i].produto);
    free(heap);

    printf("Selecao por Substituicao concluida. %d particoes geradas.\n", num_particao);
}