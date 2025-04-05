# Questão 1
Considere um grafo direcionado  G=(V,E) , representado por uma lista de adjacências, do seguinte tipo:
```console
typedef vector<int> vi;
vi* LA;
```
Escreva uma função que dada uma lista de adjacências LA, o número de vértices de  G ,  n , e um vértice  u∈V , retorne o grau de entrada e o grau de saída de  u , representados por um pair. Como saída, imprima (para cada caso de teste) o vértice  u , o grau de entrada e o grau de saída.