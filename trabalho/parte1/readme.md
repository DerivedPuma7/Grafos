# GCC262 - Grafos e suas aplicações - Trabalho Prático - Etapa 1

## Grupo:
- Nome: Gustavo Ribeiro de Figueiredo
- Matrícula: 202210846
- Professor: MAYRON CESAR DE OLIVEIRA MOREIRA

## Implementação:
- Implementação realizada em C++ e Python 3
- Versão g++: 11.4.0
- Versão Pyhon: 3.10.12
- Versão pip: 22.0.2

## Estrutura de diretórios
```
grafos/
│
├──trabalho/
    │
    ├──parte1/                      # Diretório atual
        │
        ├──definicao/               # PDF com a definição da parte 1
        ├──exemplos/                # Arquivos .dat disponibilizados
        ├──logs/                    # Arquivos .txt com logs informativos
        ├──src/                     # Arquivos com a implementação
            ├──entities/            # Contém as classes Grafo e Aresta
            ├──utils/               # Contém implementações auxiliares
            ├──main.cpp             # Arquivo de entrada
            ├──visualizacao.ipynb   # Visualização dos resultados
            ├──metricasGerais.cvs   # Arquivo csv com as métricas gerais dos grafos
            ├──intermediacoes.cvs   # Arquivo csv com as métricas das intermediações de cada vértice de cada grafo
        ├──readme.md                # Este arquivo
```

## Explicações gerais sobre a implementação
- Foi implementado uma classe Grafo responsável por representar um grafo em memória
- Método adicionarAresta(): representa um caminho bidirecional, de A → B e B → A
- Método adicionarArco(); representa um caminho direcionado, apenas de A → B
- Utilizei uma lista de adjacência para armazenar as arestas e vértices do grafo
- Utilizei uma matriz de adjacência para executar Floyd-Warshall e armazenar a Matriz W e a Matriz de Predecessores

## Visualização dos resultados
- A visualização dos dados está disponível no arquivo visualizacao.ipynb em forma de tabelas
- São como as tabelas do excel, mas com uma visualização aprimorada

## Dependências obrigatórias
- Compilador de código c++
    - Sugestão: g++
    ```
    sudo apt update
    sudo apt install g++
    g++ --version
    ```
- Python3
    ```
    sudo apt update
    sudo apt install python3
    python3 --version
    ```
- Pip (gerenciador de pacotes para o python)
    ```
    sudo apt install python3-pip
    ```
- Bibliotecas para montar a visualização dos dados
    ```
    pip install pandas matplotlib seaborn jupyter
    ```

## Como executar
- Navegue até o diretório src/ que contém o arquivo main.cpp
- Compile o arquivo:
```
g++ main.cpp -o main
```
- Execute o arquivo
    - Após isso, 2 arquivos .csv deverão ser gerados. Eles contém o resultado das métricas para cada grafo
```
./main
```
- Abra o arquivo visualizacao.ipynb e execute as células sequencialmente
