# GCC262 - Grafos e suas aplicações - Trabalho Prático - Etapa 1

## Grupo:
- Nome - Matrícula
    - Gustavo Ribeiro de Figueiredo - 202210846
    - Cesar Augusto Pires - 202210116
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
            ├──resultados/          # Contém os resultados com as rotas para cada arquivo de entrada
            ├──utils/               # Contém implementações auxiliares
            ├──main.cpp             # Arquivo de entrada
            ├──visualizacao.ipynb   # Visualização dos resultados
            ├──metricasGerais.cvs   # Arquivo csv com as métricas gerais dos grafos
            ├──intermediacoes.cvs   # Arquivo csv com as métricas das intermediações de cada vértice de cada grafo
        ├──readme.md                # Este arquivo
    ├──parte2/
        │
        ├──dados/                   # dados de entrada para a parte 2 do trabalho
        ├──solucoes/                # soluções de referência disponibilizadas pelo professor
```

## Explicações gerais sobre a implementação
- Foi implementado uma classe Grafo responsável por representar um grafo em memória
- Método adicionarAresta(): representa um caminho bidirecional, de A → B e B → A
- Método adicionarArco(); representa um caminho direcionado, apenas de A → B
- Utilizei uma lista de adjacência para armazenar as arestas e vértices do grafo
- Utilizei uma matriz de adjacência para executar Floyd-Warshall e armazenar a Matriz W e a Matriz de Predecessores

## Explicações gerais sobre a solução e otimização
- Utilizamos um algoritmo construtivo, que parte do nada e chega em uma solução viável ao problema.
- A abordagem é muito semelhante ao algoritmo de "Caminho mais Curto" para solução do TSP. Foi adaptado para o nosso problema, mas a essência é a mesma.
- Para a otimização, utilizamos a abordagem conhecida como 2-opt, que busca inverter segmentos em busca de diminuir o custo da solução.
- Colocando em termos mais práticos, tentamos inverter a ordem com que os serviços são atendidos em uma rota e verificamos se a nova rota é melhor do que a original. Caso seja, ela é aceita. Caso não, mantemos a rota original.
- ## Ressalva sobre a primeira tentativa de otimização
    - A primeira ideia que tentamos aplicar foi, quando a capacidade do veículo estivesse baixa, direcionar o veículo para um serviço intermediário entre a posição atual e o depósito, de maneira que o veículo não se afastasse ainda mais do depósito e tivesse que fazer uma viagem longa na volta ao depósito.
    - Pensando na ideia parece ser uma ótima abordagem, e que o custo fosse ser reduzido. Mas esse não foi o resultado encontrado. O custo total para praticamente todas as instâncias aumentou ao invés de diminuir.
    - Eu não sei se a implementação foi incorreta ou se a ideia realmente não funciona, mas optamos por manter essa "otimização" fora da solução final.
    - Mantemos as implementações no código para fim de documentação, mas elas não influenciam na solução final.
    - Há comentários explicativos na classe Solucao, mais especificamente nos métodos 'encontrarMelhorServico' e 'encontrarServicoIntermediarioEntreDepositoEVerticeAtual'

## Visualização dos resultados
- A visualização dos dados está disponível no arquivo visualizacao.ipynb em forma de tabelas
- São como as tabelas do excel, mas com uma visualização aprimorada
- As rotas estão disponibilizadas no diretório src/resultados

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
    - É possível passar argumentos via CLI para indicar qual arquivo deseja executar
    - Após isso, 2 arquivos .csv deverão ser gerados. Eles contém o resultado das métricas para cada grafo
```
./main ou ./main nomearquivo.dat
```
- Abra o arquivo visualizacao.ipynb e execute as células sequencialmente
