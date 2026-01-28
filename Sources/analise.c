#include <stdio.h>
#include <time.h>

#include "analise.h"
#include "produto.h"
#include "selectionSort.h"
#include "particoes.h"
#include "intercalacao.h"

#define ARQUIVO_TESTE_BASE "base_teste_temp.dat"
#define ARQUIVO_METODO_1 "teste_metodo1.dat"
#define ARQUIVO_METODO_2 "teste_metodo2.dat"

void copiar_arquivo(char *origem, char *destino) {
    FILE *f_orig = fopen(origem, "rb");
    FILE *f_dest = fopen(destino, "wb");

    if (!f_orig || !f_dest) return;

    char buffer[1024];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), f_orig)) > 0) {
        fwrite(buffer, 1, n, f_dest);
    }

    fclose(f_orig);
    fclose(f_dest);
}

void realizar_analise_experimental(FILE *arq_produtos) {
    int tamanhos[] = {10, 250, 1000, 2500};
    int num_cenarios = 4;
    int M = 10; // Memoria

    FILE *log = fopen("relatorio_analise.txt", "w");
    if (!log) {
        printf("Erro ao criar arquivo de log.\n");
        return;
    }

    fprintf(log, "=========================================================================\n");
    fprintf(log, "                   RELATORIO DE ANALISE EXPERIMENTAL                     \n");
    fprintf(log, "=========================================================================\n");
    fprintf(log, "Metodo 1: Selection Sort em Disco (Ordenacao Interna Simulada)\n");
    fprintf(log, "Metodo 2: Ordenacao Externa (Selecao por Substituicao + Intercalacao Otima)\n");
    fprintf(log, "-------------------------------------------------------------------------\n");
    fprintf(log, "| %-10s | %-25s | %-25s |\n", "Qtd Reg.", "Tempo Metodo 1 (s)", "Tempo Metodo 2 (s)");
    fprintf(log, "-------------------------------------------------------------------------\n");

    printf("\n=== Iniciando Analise Experimental ===\n");
    printf("Gerando cenarios de teste e processando ordenacoes...\n\n");

    for (int i = 0; i < num_cenarios; i++) {
        int tam = tamanhos[i];

        printf(" >> Processando cenario com %d registros... ", tam);
        fflush(stdout);

        FILE *f_base = fopen(ARQUIVO_TESTE_BASE, "wb+");
        criar_base_produto(f_base, tam, 0);
        fclose(f_base);

        //parte1
        copiar_arquivo(ARQUIVO_TESTE_BASE, ARQUIVO_METODO_1);
        FILE *f_m1 = fopen(ARQUIVO_METODO_1, "r+b");

        clock_t inicio1 = clock();
        selection_sort_disco_produto(f_m1);
        clock_t fim1 = clock();
        fclose(f_m1);

        double tempo1 = (double)(fim1 - inicio1) / CLOCKS_PER_SEC;

        //parte2
        copiar_arquivo(ARQUIVO_TESTE_BASE, ARQUIVO_METODO_2);
        FILE *f_m2 = fopen(ARQUIVO_METODO_2, "r+b");

        clock_t inicio2 = clock();
        selecao_substituicao(f_m2, "teste_part", M);
        intercalacao_otima("teste_part", ARQUIVO_METODO_2);
        clock_t fim2 = clock();
        if (f_m2) fclose(f_m2);

        double tempo2 = (double)(fim2 - inicio2) / CLOCKS_PER_SEC;

        fprintf(log, "| %-10d | %-25.4f | %-25.4f |\n", tam, tempo1, tempo2);

        printf("Concluido.\n");

        remove(ARQUIVO_TESTE_BASE);
        remove(ARQUIVO_METODO_1);
        remove(ARQUIVO_METODO_2);
    }

    fprintf(log, "-------------------------------------------------------------------------\n");
    fclose(log);

    printf("\nAnalise finalizada com sucesso!\n");
    printf("Resultados detalhados disponiveis em 'relatorio_analise.txt'.\n");

    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}