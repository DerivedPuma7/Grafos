# Explicação sobre o min heap

```console
priority_queue<Tipo, Container, Comparador> nome;
```

- Tipo: o tipo dos elementos armazenados na fila
- Container: a estrutura usada para armazenar os elementos (normalmente um vector)
- Comparador: a função usada para determinar a ordem dos elementos

## Tipo dos elementos
```console
tuple<int, int, int>
```
Isso significa que cada elemento da fila é uma tupla com três inteiros, representando:

- Peso da aresta (int) → Prioridade (menor peso primeiro)
- Vértice de origem (int)
- Vértice de destino (int)

Exemplo:
```console
fila.push({3, 1, 2}); // Aresta de peso 3 entre os vértices 1 e 2
```

## Container
```console
vector<tuple<int, int, int>>
```
Isso define que a estrutura de armazenamento da priority_queue será um vector

## Comparador
```console
greater<>
```
Este é o comparador que inverte a ordem da fila de prioridade.
Por padrão, a priority_queue em C++ é um max-heap (maiores valores primeiro).
Para transformar em min-heap (menores valores primeiro), usamos greater<>.