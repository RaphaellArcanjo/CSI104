#include <stdio.h>
#include <stdlib.h>

#include "interface.h"

int main() {
    FILE *arq_fornecedores = NULL;
    FILE *arq_produtos = NULL;
    int op = -1;

    arq_fornecedores = fopen(ARQUIVO_FORNECEDORES, "r+b");
    arq_produtos = fopen(ARQUIVO_PRODUTOS, "r+b");

    do {
        limpa_tela();
        MSG_MENU();
        scanf("%d", &op);
        limpa_buffer_stdin();

        switch (op) {
            case 1:
                limpa_tela();
                //passa o endereco dos ponteiros
                op_gerar_base(&arq_fornecedores, &arq_produtos);
                pausa();
                break;
            case 2:
                limpa_tela();
                op_imprimir_bases(arq_fornecedores, arq_produtos);
                pausa();
                break;
            case 3:
                limpa_tela();
                op_ordenar_bases(arq_fornecedores, arq_produtos);
                pausa();
                break;
            case 4:
                limpa_tela();
                op_busca_sequencial(arq_fornecedores, arq_produtos);
                pausa();
                break;
            case 5:
                limpa_tela();
                op_busca_binaria(arq_fornecedores, arq_produtos);
                pausa();
                break;
            case 6:
                limpa_tela();
                op_listar_produtos_por_fornecedor(arq_fornecedores, arq_produtos);
                pausa();
                break;
            case 7:
                limpa_tela();
                op_atualizar_preco_produto(arq_produtos);
                pausa();
                break;
            //PARTE 2
            case 8:
                limpa_tela();
                op_gerar_particoes(arq_produtos);
                pausa();
                break;
            case 9:
                limpa_tela();
                if (arq_produtos) { fclose(arq_produtos); arq_produtos = NULL; }

                op_intercalacao_otima(arq_produtos);

                arq_produtos = fopen(ARQUIVO_PRODUTOS, "r+b");
                if (!arq_produtos) {
                    printf("Aviso: Base de produtos fechada ou inexistente.\n");
                }
                pausa();
                break;
            case 10:
                limpa_tela();
                op_analise_experimental(arq_produtos);
                pausa();
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                pausa();
                break;
        }

    } while (op != 0);

    //fecha todos os arquivos abertos
    if (arq_fornecedores) {
        fclose(arq_fornecedores);
    }
    if (arq_produtos) {
        fclose(arq_produtos);
    }

    return 0;
}