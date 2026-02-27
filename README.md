# Algoritmos e Estruturas de Dados II (AEDS II) - Trabalho Prático

Este repositório contém a implementação dos trabalhos práticos da disciplina de Algoritmos e Estruturas de Dados II. O projeto consiste em um sistema de gerenciamento de **Produtos** e **Fornecedores**, focado na manipulação de arquivos binários, algoritmos de ordenação externa e estruturas de dados avançadas.

---

## Entidades

O sistema trabalha com duas entidades principais relacionadas:
- **Fornecedor**: Possui ID, Razão Social e CNPJ.
- **Produto**: Possui ID, Nome, Preço, Estoque e uma chave estrangeira para o Fornecedor.

---

## Parte I: Busca e Ordenação em Disco

Nesta fase inicial, o foco foi a criação das bases de dados em arquivos binários e a implementação de métodos fundamentais de acesso aos dados.

- **Criação de Bases**: Geração de arquivos binários desordenados (`fornecedores.dat` e `produtos.dat`).
- **Buscas**:
    - **Busca Sequencial**: Percorre o arquivo registro por registro.
    - **Busca Binária**: Realizada em arquivos previamente ordenados, permitindo acesso rápido aos dados.
- **Ordenação em Disco**: Implementação do algoritmo **Selection Sort** para ordenar os registros diretamente no arquivo binário.
- **Interação**: Funcionalidades que exemplificam a relação entre produtos e seus respectivos fornecedores.

---

## Parte II: Ordenação Externa

A segunda parte focou em algoritmos de ordenação para grandes volumes de dados que não cabem na memória principal.

- **Geração de Partições**: Implementação do método de **Seleção por Substituição**.
- **Intercalação**: Implementação da **Intercalação Ótima** para unir as partições geradas em um único arquivo ordenado.
- **Análise de Desempenho**: Comparação entre o método de ordenação interna (Selection Sort) e a ordenação externa, com geração de logs de tempo e número de comparações/acessos ao disco.

---

## Parte III: Gerenciamento de Arquivos com Tabela Hash

A última etapa envolveu o gerenciamento eficiente de arquivos utilizando estruturas indexadas.

- **Tabela Hash**: Implementação de uma Tabela Hash em disco para a entidade **Produto**.
- **Operações**:
    - Inserção de novos produtos.
    - Busca rápida por chave primária (ID).
    - Remoção de registros.
- **Gerenciamento de Espaço**: Técnica para recuperar e reutilizar espaços vazios deixados por registros removidos, otimizando o tamanho do arquivo binário.

---

### Estrutura de Pastas
- `Headers/`: Arquivos de cabeçalho (`.h`).
- `Sources/`: Implementações dos algoritmos e lógica de negócio (`.c`).

---