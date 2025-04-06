# GCC262 - Grafos e suas aplicações - Trabalho Prático - Etapa 1

## Grupo:
- Nome: Gustavo Ribeiro de Figueiredo
- Matrícula: 202210846
- Professor: MAYRON CESAR DE OLIVEIRA MOREIRA

## Implementação:
- Implementação realizada em C++
- Versão g++: 11.4.0

## Estrutura de diretórios
```
grafos/
│
├──trabalho/
    │
    ├──parte1/                  # Diretório atual
        │
        ├──definicao/           # PDF com a definição da parte 1
        ├──exemplos/            # Arquivos .dat disponibilizados
        ├──implementacao.cpp    # Arquivo com a implementação
        ├──readme.md            # Este arquivo
```

## Explicações gerais sobre a implementação
- Foi implementado uma classe Grafo responsável por representar um grafo em memória
- Método adicionarAresta(): representa um caminho bidirecional, de A → B e B → A
- Método adicionarArco(); representa um caminho direcionado, apenas de A → B
- Utilizei uma lista de adjacência para armazenar as arestas e vértices do grafo
- Utilizei uma matriz de adjacência para executar Floyd-Warshall e armazenar a Matriz W e a Matriz de Predecessores
- Aviso "Implementado com IA ⚠️". Isso indica que utilizei IAs generativas para ajudar na implementação desses métodos. Os demais métodos tentei ao máximo seguir sozinho.

## Explicação sobre entrada dos dados
- Mayron, não entendi muito bem como interpretar esses arquivos .dat e não sabia o que fazer com eles
- Eu imagino que cada um represente um grafo, e a partir desse grafo, deveríamos obter as métricas listadas
- Mas falhei em interpretar esses arquivos

## Como executar
- No arquivo implementação.cpp, na função main(), basta inicializar o grafo e montar o caminho das arestas/arcos (leia o item 2 do tópico "Explicações gerais sobre a implementação")
- Montar cada grafo manualmente, especialmente se for um grafo grande, pode ser bem trabalhoso, mas não consegui ler os arquivos de entrada
- Compile o arquivo:
```
g++ implementacao.cpp -o executavel
```
- Execute o arquivo
```
./executavel
```
