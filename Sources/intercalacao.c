#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intercalacao.h"
#include "produto.h"

typedef struct {
    char nome[100];
    long tamanho_bytes;
} TArquivoParticao;

int compara_tamanho(const void *a, const void *b) {
    TArquivoParticao *arg1 = (TArquivoParticao *) a;
    TArquivoParticao *arg2 = (TArquivoParticao *) b;

    if (arg1->tamanho_bytes < arg2->tamanho_bytes) return -1;
    if (arg1->tamanho_bytes > arg2->tamanho_bytes) return 1;
    return 0;
}

long pegar_tamanho_arquivo(char *nome) {
    FILE *f = fopen(nome, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fclose(f);
    return tam;
}

int intercalar_dois_arquivos(char *nome_arq1, char *nome_arq2, char *nome_saida) {
    FILE *f1 = fopen(nome_arq1, "rb");
    FILE *f2 = fopen(nome_arq2, "rb");
    FILE *saida = fopen(nome_saida, "wb");

    if (!f1 || !f2 || !saida) {
        printf("Erro ao abrir arquivos para intercalacao.\n");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        if (saida) fclose(saida);
        return 0;
    }

    printf("   Intercalando: %s + %s -> %s\n", nome_arq1, nome_arq2, nome_saida);

    TProduto *p1 = (TProduto *) malloc(sizeof(TProduto));
    TProduto *p2 = (TProduto *) malloc(sizeof(TProduto));

    int tem_p1 = le_produto_no_heap(f1, p1);
    int tem_p2 = le_produto_no_heap(f2, p2);

    while (tem_p1 && tem_p2) {
        if (p1->id_produto < p2->id_produto) {
            salva_produto(p1, saida);//o menor vai pro arquivo final
            tem_p1 = le_produto_no_heap(f1, p1);
        } else {
            salva_produto(p2, saida);
            tem_p2 = le_produto_no_heap(f2, p2);
        }
    }

    while (tem_p1) {
        salva_produto(p1, saida);
        tem_p1 = le_produto_no_heap(f1, p1);
    }

    while (tem_p2) {
        salva_produto(p2, saida);
        tem_p2 = le_produto_no_heap(f2, p2);
    }

    free(p1);
    free(p2);
    fclose(f1);
    fclose(f2);
    fclose(saida);
    return 1;
}

void intercalacao_otima(char *prefixo_particoes, char *nome_arquivo_final) {
    printf("\n=== Iniciando Intercalacao Otima ===\n");

    TArquivoParticao lista[1000];
    int qtd = 0;
    char nome_temp[100];

    int i = 1;
    while (1) {
        sprintf(nome_temp, "%s%d.dat", prefixo_particoes, i);
        FILE *f = fopen(nome_temp, "rb");
        if (f) {
            fclose(f);
            strcpy(lista[qtd].nome, nome_temp);
            lista[qtd].tamanho_bytes = pegar_tamanho_arquivo(nome_temp);
            qtd++;
            i++;
        } else {
            break;
        }
    }

    if (qtd == 0) {
        printf("Nenhuma particao encontrada para intercalar.\n");
        return;
    }

    printf("Encontradas %d particoes iniciais.\n", qtd);

    int contador_temps = 0;

    while (qtd > 1) {
        qsort(lista, qtd, sizeof(TArquivoParticao), compara_tamanho);

        TArquivoParticao menor1 = lista[0];
        TArquivoParticao menor2 = lista[1];

        contador_temps++;
        sprintf(nome_temp, "temp_merge_%d.dat", contador_temps);

        if (intercalar_dois_arquivos(menor1.nome, menor2.nome, nome_temp)) {
            remove(menor1.nome);
            remove(menor2.nome);

            strcpy(lista[0].nome, nome_temp);
            lista[0].tamanho_bytes = pegar_tamanho_arquivo(nome_temp);

            for (int k = 1; k < qtd - 1; k++) {
                lista[k] = lista[k+1];
            }
            qtd--;
        } else {
            printf("Erro fatal na intercalacao.\n");
            return;
        }
    }

    remove(nome_arquivo_final);

    if (rename(lista[0].nome, nome_arquivo_final) == 0) {
        printf("\nIntercalacao concluida com sucesso!\n");
        printf("Arquivo ordenado gerado: %s\n", nome_arquivo_final);
    } else {
        printf("\nAtencao: Nao foi possivel renomear para '%s'.\n", nome_arquivo_final);
        printf("O arquivo ordenado final encontra-se em: %s\n", lista[0].nome);
    }
}