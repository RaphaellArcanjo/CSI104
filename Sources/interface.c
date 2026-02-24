#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "interface.h"
#include "analise.h"
#include "fornecedor.h"
#include "produto.h"
#include "selectionSort.h"
#include "buscaSequencialFornecedor.h"
#include "buscaSequencialProduto.h"
#include "buscaBinariaFornecedor.h"
#include "buscaBinariaProduto.h"
#include "intercalacao.h"
#include "particoes.h"
#include "tabelaHash.h"


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
    //PARTE 2
    printf("8. Gerar Particoes (Selecao por Substituicao)\n");
    printf("9. Intercalacao Otima (Juntar Particoes)\n");
    printf("10. Analise\n");
    //PARTE 3
    printf("11. Tabela Hash (Inserir/Remover/Buscar)\n");
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
void op_gerar_base(FILE **arq_f, FILE **arq_p, FILE **arq_h, int *M) {
    int tam_forn, tam_prod;

    printf("--- Gerando Bases de Dados ---\n");
    printf("AVISO: Isso apagara todos os dados existentes (Sequencial e Hash).\n");

    printf("1. Quatidade Fornecedores: ");
    scanf("%d", &tam_forn);
    printf("2. Quantidade Produtos: ");
    scanf("%d", &tam_prod);

    // Pergunta o tamanho da Hash
    printf("3. Tamanho da Tabela Hash (M) (Recomendado > %d): ", tam_prod);
    scanf("%d", M);
    limpa_buffer_stdin();

    if (*arq_f) fclose(*arq_f);
    *arq_f = fopen(ARQUIVO_FORNECEDORES, "w+b");
    criar_base_fornecedor(*arq_f, tam_forn);
    int total_forn = tamanho_arquivo_fornecedor(*arq_f);

    if (*arq_p) fclose(*arq_p);
    *arq_p = fopen(ARQUIVO_PRODUTOS, "w+b");
    criar_base_produto(*arq_p, tam_prod, total_forn);

    fclose(*arq_f); fclose(*arq_p);
    *arq_f = fopen(ARQUIVO_FORNECEDORES, "r+b");
    *arq_p = fopen(ARQUIVO_PRODUTOS, "r+b");

    printf("\nBases sequenciais geradas!\n");

    printf("\n--- Migrando dados para Tabela Hash (Primeiros slots embaralhados) ---\n");

    if (*arq_h) fclose(*arq_h);
    *arq_h = fopen("tabela_hash.dat", "w+b"); 

    if (*arq_h) {
        inicializar_tabela_hash(*arq_h, *M);

        rewind(*arq_p);
        TProduto **vetor_produtos = (TProduto**) malloc(tam_prod * sizeof(TProduto*));
        for (int i = 0; i < tam_prod; i++) {
            vetor_produtos[i] = le_produto(*arq_p);
        }

        for (int i = 0; i < tam_prod; i++) {
            int j = rand() % tam_prod;
            TProduto *temp = vetor_produtos[i];
            vetor_produtos[i] = vetor_produtos[j];
            vetor_produtos[j] = temp;
        }

        rewind(*arq_h);
        for (int i = 0; i < tam_prod; i++) {
            if (i < *M) {
                salva_produto(vetor_produtos[i], *arq_h);
            }
            free(vetor_produtos[i]);
        }
        free(vetor_produtos);

        printf("Sucesso! %d produtos inseridos sequencialmente (posicoes 0 a %d) de forma embaralhada.\n", 
                tam_prod < *M ? tam_prod : *M, tam_prod < *M ? tam_prod - 1 : *M - 1);
    } else {
        printf("Erro ao criar arquivo hash.\n");
    }
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

//PARTE 2
//case8
void op_gerar_particoes(FILE *arq_p) {
    if (arq_p == NULL) {
        printf("ERRO: Voce precisa gerar a base de dados primeiro.\n");
        return;
    }

    printf("--- Gerar Particoes Ordenadas ---\n");
    printf("Digite o tamanho da memoria (M) para o Heap (ex: 6): ");
    int M;
    scanf("%d", &M);
    limpa_buffer_stdin();

    if (M < 2) {
        printf("Erro: O tamanho da memoria deve ser pelo menos 2.\n");
        return;
    }

    selecao_substituicao(arq_p, "particao", M);
}

//case 9
void op_intercalacao_otima(FILE *arq_p) {

    printf("--- Intercalacao Otima ---\n");
    printf("Esta opcao vai procurar por arquivos 'particao1.dat', 'particao2.dat'...\n");
    printf("E gerar um arquivo final 'produtos_ordenados.dat'.\n\n");

    FILE *teste = fopen("particao1.dat", "rb");
    if (!teste) {
        printf("ERRO: Nenhuma particao encontrada. Rode a Opcao 8 primeiro.\n");
        return;
    }
    fclose(teste);

    intercalacao_otima("particao", "produtos_ordenados.dat");

    printf("\nDeseja substituir a base oficial 'produtos.dat' pelo arquivo ordenado? (1-Sim / 0-Nao): ");
    int resp;
    scanf("%d", &resp);
    limpa_buffer_stdin();

    if (resp == 1) {
        if (arq_p) fclose(arq_p);

        remove(ARQUIVO_PRODUTOS);
        rename("produtos_ordenados.dat", ARQUIVO_PRODUTOS);

        printf("Base de dados oficial atualizada e ordenada!\n");
        printf("NOTA: A base foi reaberta automaticamente pelo sistema.\n");
    }

}

//case 10
void op_analise_experimental(FILE *arq_p) {
    realizar_analise_experimental(NULL);
}


//PARTE 3
//case 11
void op_tabela_hash(FILE **arq_hash, int *M) {
    int op = -1;
    const char *NOME_ARQ_HASH = "tabela_hash.dat";

    do {
        printf("\n=== MENU TABELA HASH ===\n");
        if (*M == 0) printf("AVISO: Tabela nao inicializada (M=0). Gere a base na Opcao 1.\n");
        else printf("Arquivo: %s (Tamanho M: %d)\n", NOME_ARQ_HASH, *M);

        printf("1. Criar/Reinicializar Tabela\n");
        printf("2. Inserir Produto (Novo)\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("5. Imprimir Tabela\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &op);
        limpa_buffer_stdin();

        switch (op) {
            case 1:
                printf("Novo tamanho M: ");
                scanf("%d", M);
                if (*arq_hash) fclose(*arq_hash);
                *arq_hash = fopen(NOME_ARQ_HASH, "w+b");
                inicializar_tabela_hash(*arq_hash, *M);
                break;

            case 2: {
                if (*M == 0) {
                    printf("Gere a base primeiro.\n");
                    break;
                }

                int pos_desejada;
                printf("\nDigite a POSICAO que deseja inserir (0 a %d): ", *M - 1);
                scanf("%d", &pos_desejada);
                limpa_buffer_stdin();

                if (pos_desejada < 0 || pos_desejada >= *M) {
                    printf("ERRO: Posicao invalida.\n");
                    break;
                }

                int id_novo;
                int id_valido = 0;
                while (1) {
                    printf("Digite o ID do novo produto (-1 para voltar): ");
                    scanf("%d", &id_novo);
                    limpa_buffer_stdin();

                    if (id_novo == -1) {
                        break;
                    }

                    TProduto *p_check = busca_hash(*arq_hash, id_novo, *M);
                    if (p_check != NULL) {
                        printf("ERRO: O ID %d ja existe na tabela. Tente outro.\n", id_novo);
                        free(p_check);
                    } else {
                        id_valido = 1;
                        break;
                    }
                }

                if (id_valido) {
                    TProduto *novo = (TProduto*) malloc(sizeof(TProduto));
                    novo->id_produto = id_novo;

                    printf("Nome: ");
                    fgets(novo->nome_produto, sizeof(novo->nome_produto), stdin);
                    novo->nome_produto[strcspn(novo->nome_produto, "\n")] = 0;

                    printf("Preco: ");
                    scanf("%lf", &novo->preco);

                    printf("Estoque: ");
                    scanf("%d", &novo->estoque);

                    printf("ID Fornecedor: ");
                    scanf("%d", &novo->id_fornecedor);
                    limpa_buffer_stdin();

                    int pos_real = inserir_hash(*arq_hash, novo, *M, pos_desejada);

                    if (pos_real != -1) {
                        printf("\nSucesso! Produto inserido na posicao %d.\n", pos_real);
                    } else {
                        printf("\nERRO: Nao foi possivel inserir. A tabela pode estar cheia a partir da posicao %d.\n", pos_desejada);
                    }
                    free(novo);
                }
                break;
            }

            case 3:
                if (*M == 0) break;
                int id_b;
                printf("ID Busca: "); scanf("%d", &id_b);
                limpa_buffer_stdin();

                TProduto *pb = busca_hash(*arq_hash, id_b, *M);
                if (pb) { 
                    printf("\n--- Produto Encontrado ---\n");
                    imprime_produto(pb); 
                    free(pb); 
                }
                else printf("\nErro: ID %d nao encontrado.\n", id_b);
                break;

            case 4:
                if (*M == 0) break;
                int id_r;
                printf("ID Remover: "); scanf("%d", &id_r);
                limpa_buffer_stdin();

                TProduto *p_rem = busca_hash(*arq_hash, id_r, *M);
                if (p_rem) {
                    printf("\n--- Registro a ser removido ---\n");
                    imprime_produto(p_rem);
                    free(p_rem);
                    
                    remover_hash(*arq_hash, id_r, *M);
                } else {
                    printf("\nErro: ID %d nao encontrado para remocao.\n", id_r);
                }
                break;

            case 5:
                if (*M == 0) break;
                imprimir_tabela_hash(*arq_hash, *M);
                break;
        }
        if (op != 0) pausa();
        limpa_tela();
    } while (op != 0);
}