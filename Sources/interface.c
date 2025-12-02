#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface.h"
#include "fornecedor.h"
#include "produto.h"
#include "selectionSort.h"
#include "buscaSequencialFornecedor.h"
#include "buscaSequencialProduto.h"
#include "buscaBinariaFornecedor.h"
#include "buscaBinariaProduto.h"


void MSG_MENU() {
    printf("===========================================================\n");
    printf("Sistema de Cadastro de Produtos e Fornecedores \n");
    printf("===========================================================\n");
    printf("1. Gerar Base de Dados (ZERA OS DADOS!)\n");
    printf("2. Imprimir Base de Dados\n");
    printf("3. Ordenar Base de Dados (Selection Sort)\n");
    printf("4. Busca Sequencial\n");
    printf("5. Busca Binaria\n");
    printf("6. Listar Produtos por Fornecedor\n");
    printf("7. Atualizar Preco de Produto\n");
    printf("0. Sair\n");
    printf("===========================================================\n");
    printf("Escolha uma opcao: ");
}

void limpa_tela() {
    system("clear || cls");
}

void limpa_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausa() {
    printf("\nPressione Enter para continuar...");
    getchar();
}


//case1
void op_gerar_base(FILE **arq_f, FILE **arq_p) {
    int tam_forn, tam_prod;

    printf("--- Gerando Bases de Dados ---\n");
    printf("AVISO: Isso apagara todos os dados existentes.\n");

    printf("Digite o tamanho da base de FORNECEDOR que deseja gerar: ");
    scanf("%d", &tam_forn);
    limpa_buffer_stdin();

    printf("\nDigite o tamanho da base de PRODUTO que deseja gerar: ");
    scanf("%d", &tam_prod);
    limpa_buffer_stdin();

    if (tam_forn <= 0 || tam_prod <= 0) {
        printf("\nERRO: Os tamanhos devem ser maiores que zero.\n");
        return;
    }


    if (*arq_f) fclose(*arq_f);
    if (*arq_p) fclose(*arq_p);

    *arq_f = fopen(ARQUIVO_FORNECEDORES, "w+b");
    if (*arq_f == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo de fornecedores.\n");
        pausa();
        return;
    }
    criar_base_fornecedor(*arq_f, tam_forn);

    int total_forn = tamanho_arquivo_fornecedor(*arq_f);
    printf("Base de fornecedores criada com %d registros.\n", total_forn);

    *arq_p = fopen(ARQUIVO_PRODUTOS, "w+b");
    if (*arq_p == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo de produtos.\n");
        pausa();
        return;
    }
    criar_base_produto(*arq_p, tam_prod, total_forn);
    printf("Base de produtos criada e linkada aos fornecedores.\n");

    fclose(*arq_f);
    fclose(*arq_p);
    *arq_f = fopen(ARQUIVO_FORNECEDORES, "r+b");
    *arq_p = fopen(ARQUIVO_PRODUTOS, "r+b");

    printf("\nBases de dados geradas com sucesso.\n");
}

//case2
void op_imprimir_bases(FILE *arq_f, FILE *arq_p) {
    if (arq_f == NULL || arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro (Opcao 1).\n");
        pausa();
        return;
    }
    printf("1. Imprimir Fornecedores\n");
    printf("2. Imprimir Produtos\n");
    printf("Escolha uma opcao: ");
    int sub_op;
    scanf("%d", &sub_op);
    printf("\n===========================================================\n");
    limpa_buffer_stdin();
    switch (sub_op) {
        case 1:
            printf("\n--- Lista de Fornecedores ---\n");
            imprimir_base_fornecedor(arq_f);
            break;
        case 2:
            printf("\n--- Lista de Produtos ---\n");
            imprimir_base_produto(arq_p);
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

//case3
void op_ordenar_bases(FILE *arq_f, FILE *arq_p) {
    if (arq_f == NULL || arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro (Opcao 1).\n");
        pausa();
        return;
    }

    clock_t inicio_tempo, fim_tempo;
    double tempo_gasto;
    int total_comparacoes;

    printf("1. Ordenar Fornecedores (por ID)\n");
    printf("2. Ordenar Produtos (por ID)\n");
    printf("Escolha uma opcao: ");
    int sub_op;
    scanf("%d", &sub_op);
    printf("\n===========================================================\n");
    limpa_buffer_stdin();

    switch (sub_op) {
        case 1:
            printf("Ordenando fornecedores... (aguarde)\n");
            inicio_tempo = clock();
            total_comparacoes = selection_sort_disco_fornecedor(arq_f);
            fim_tempo = clock();
            tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;

            printf("Base de fornecedores ordenada com sucesso.\n\n");
            printf("--- Analise de Desempenho ---\n");
            printf("Tempo gasto: %.4f segundos\n", tempo_gasto);
            printf("Comparacoes de chave: %d\n", total_comparacoes);
            printf("===========================================================\n");
            break;
        case 2:
            printf("Ordenando produtos... (aguarde)\n");
            inicio_tempo = clock();
            total_comparacoes = selection_sort_disco_produto(arq_p);
            fim_tempo = clock();
            tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;

            printf("Base de produtos ordenada com sucesso.\n\n");
            printf("--- Analise de Desempenho ---\n");
            printf("Tempo gasto: %.4f segundos\n", tempo_gasto);
            printf("Comparacoes de chave: %d\n", total_comparacoes);
            printf("===========================================================\n");
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

//case4
void op_busca_sequencial(FILE *arq_f, FILE *arq_p) {
    if (arq_f == NULL || arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro (Opcao 1).\n");
        pausa();
        return;
    }

    clock_t inicio_tempo, fim_tempo;
    double tempo_gasto;
    int total_comparacoes = 0;

    printf("1. Buscar Fornecedor (por ID)\n");
    printf("2. Buscar Produto (por ID)\n");
    printf("Escolha uma opcao: ");
    int sub_op;
    scanf("%d", &sub_op);
    printf("\n===========================================================\n");
    limpa_buffer_stdin();

    int id_chave;
    printf("Digite o ID (chave) que deseja buscar: ");
    scanf("%d", &id_chave);
    limpa_buffer_stdin();

    switch (sub_op){
        case 1:
            printf("Buscando fornecedor ID %d...\n", id_chave);
            inicio_tempo = clock();
            TFornecedor *f_encontrado = busca_sequencial_fornecedor(id_chave, arq_f, &total_comparacoes);
            fim_tempo = clock();
            if (f_encontrado != NULL) {
                printf("\n--- Registro Encontrado! ---\n");
                imprime_fornecedor(f_encontrado);
                free(f_encontrado);
            } else {
                printf("\nRegistro (ID %d) nao encontrado.\n", id_chave);
            }
            break;
        case 2:
            printf("Buscando produto ID %d...\n", id_chave);
            inicio_tempo = clock();
            TProduto *p_encontrado = busca_sequencial_produto(id_chave, arq_p, &total_comparacoes);
            fim_tempo = clock();
            if (p_encontrado != NULL) {
                printf("\n--- Registro Encontrado! ---\n");
                imprime_produto(p_encontrado);
                free(p_encontrado);
            } else {
                printf("\nRegistro (ID %d) nao encontrado.\n", id_chave);
            }
            break;
        default:
            printf("Opcao invalida.\n");
            pausa();
            return;
    }

    tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
    printf("\n--- Analise da Busca ---\n");
    printf("Tempo gasto: %.6f s\n", tempo_gasto);
    printf("Comparacoes de chave: %d\n", total_comparacoes);
}

//case5
void op_busca_binaria(FILE *arq_f, FILE *arq_p) {
    if (arq_f == NULL || arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro (Opcao 1).\n");
        pausa();
        return;
    }

    clock_t inicio_tempo, fim_tempo;
    double tempo_gasto;
    int total_comparacoes = 0;

    printf("IMPORTANTE: Esta busca SO FUNCIONA se a base estiver ORDENADA (Opcao 3).\n\n");
    printf("===========================================================\n");
    printf("1. Buscar Fornecedor (por ID)\n");
    printf("2. Buscar Produto (por ID)\n");
    printf("Escolha uma opcao: ");

    int sub_opB;
    scanf("%d", &sub_opB);
    printf("\n===========================================================\n");
    limpa_buffer_stdin();

    int id_chaveB;
    printf("Digite o ID (chave) que deseja buscar: ");
    scanf("%d", &id_chaveB);
    limpa_buffer_stdin();

    switch (sub_opB) {
        case 1:
            printf("Buscando fornecedor ID %d...\n", id_chaveB);
            int N_forn = tamanho_arquivo_fornecedor(arq_f);
            inicio_tempo = clock();
            TFornecedor *f_encontrado = busca_binaria_fornecedor(id_chaveB, arq_f, 0, N_forn - 1, &total_comparacoes);
            fim_tempo = clock();
            if (f_encontrado != NULL) {
                printf("\n--- Registro Encontrado! ---\n");
                imprime_fornecedor(f_encontrado);
                free(f_encontrado);
            } else {
                printf("\nRegistro (ID %d) nao encontrado.\n", id_chaveB);
            }
            break;
        case 2:
            printf("Buscando produto ID %d...\n", id_chaveB);
            int N_prod = tamanho_arquivo_produto(arq_p);
            inicio_tempo = clock();
            TProduto *p_encontrado = busca_binaria_produto(id_chaveB, arq_p, 0, N_prod - 1, &total_comparacoes);
            fim_tempo = clock();
            if (p_encontrado != NULL) {
                printf("\n--- Registro Encontrado! ---\n");
                imprime_produto(p_encontrado);
                free(p_encontrado);
            } else {
                printf("\nRegistro (ID %d) nao encontrado.\n", id_chaveB);
            }
            break;
        default:
            printf("Opcao invalida.\n");
            pausa();
            return;
    }

    tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
    printf("\n--- Analise da Busca ---\n");
    printf("Tempo gasto: %.6f segundos\n", tempo_gasto);
    printf("Comparacoes de chave: %d\n", total_comparacoes);
}

//case6
void op_listar_produtos_por_fornecedor(FILE *arq_f, FILE *arq_p) {
    if (arq_f == NULL || arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro (Opcao 1).\n");
        return;
    }

    printf("--- Listar Produtos por Fornecedor ---\n");
    printf("Digite o ID do Fornecedor que deseja consultar: ");
    int id_forn_busca;
    scanf("%d", &id_forn_busca);
    limpa_buffer_stdin();

    int comparacoes_dummy = 0;

    TFornecedor *f_check = busca_sequencial_fornecedor(id_forn_busca, arq_f, &comparacoes_dummy);

    if (f_check == NULL) {
        printf("\nERRO: Nao existe um fornecedor com o ID %d.\n", id_forn_busca);
        return;
    }

    printf("\n--- Exibindo produtos do Fornecedor: ---\n");
    printf("\n===========================================================\n");
    free(f_check);

    TProduto *p;
    int encontrados = 0;

    rewind(arq_p);

    while ((p = le_produto(arq_p)) != NULL) {

        if (p->id_fornecedor == id_forn_busca) {
            imprime_produto(p);
            encontrados++;
        }

        free(p);
    }

    if (encontrados == 0) {
        printf("\nNenhum produto encontrado para este fornecedor.\n");
    } else {
        printf("\nTotal de %d produto(s) encontrados.\n", encontrados);
    }
}

//case7
void op_atualizar_preco_produto(FILE *arq_p) {
    if (arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro (Opcao 1).\n");
        return;
    }

    printf("--- Atualizar Preco de Produto ---\n");
    printf("Digite o ID do Produto a ser atualizado: ");
    int id_busca;
    scanf("%d", &id_busca);
    limpa_buffer_stdin();

    TProduto *p = NULL;
    int tam_reg = tamanho_registro_produto();

    long long pos_bytes = -1;

    rewind(arq_p);

    while ((p = le_produto(arq_p)) != NULL) {

        if (p->id_produto == id_busca) {
            pos_bytes = ftell(arq_p) - tam_reg;
            break;
        }

        free(p);
    }

    if (pos_bytes != -1) {
        printf("\n--- Produto Encontrado ---\n");
        imprime_produto(p);

        printf("Digite o novo preco: R$ ");
        double novo_preco;
        scanf("%lf", &novo_preco);
        limpa_buffer_stdin();

        p->preco = novo_preco;

        fseek(arq_p, pos_bytes, SEEK_SET);

        salva_produto(p, arq_p);

        printf("\nPreco atualizado com sucesso!\n");

        free(p);
    } else {
        printf("\nERRO: Produto com ID %d nao encontrado.\n", id_busca);
    }
}
