#include <iostream>
#include <queue>
#include <vector>
#include <tuple>

using namespace std;

class Grafo {
public:
  int numeroVertices;
  vector<vector<pair<int, int>>> listaAdjacencia; // lista de adjacência (peso, vizinho)
  /*
  listaAdjacencia
  ↓
  [0] → [{ 1, 1 }, { 2, 2 }]
  [1] → []
  [2] → []
  ...
  */

  Grafo(int v) {
    this->numeroVertices = v;
    listaAdjacencia.resize(v);
  }

  // implementação para um grafo nao direcionado (u->v e v->u)
  void adicionarAresta(int u, int v, int peso) { // vertice u------peso------vertice v
    cout << "Adicionando aresta " << u << " - " << v << " (Peso: " << peso << ")\n";
    listaAdjacencia[u].push_back({peso, v});
    listaAdjacencia[v].push_back({peso, u});
  }

  void imprimir() {
    for (int i = 0; i < numeroVertices; i++) {
      cout << i << " → ";
      for (auto [peso, vizinho] : listaAdjacencia[i]) {
        cout << "{" << peso << ", " << vizinho << "} ";
      }
      cout << endl;
    }
  }
};

vector<tuple<int, int, int>> prim(Grafo& grafo, int verticeInicial) {
  vector<tuple<int, int, int>> mst; // lista de arestas da MST (u, v, peso)
  vector<bool> visitado(grafo.numeroVertices, false); // alocamos a quantidade de vertices e inicializamos com false para cada posição do vector
  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> fila; // min-heap

  // adicionamos a vizinhança do vertice inicial na fila, e vamos seguir o algoritmo a partir dele
  visitado[verticeInicial] = true;
  for (auto [peso, vizinho] : grafo.listaAdjacencia[verticeInicial]) {
    fila.push({peso, verticeInicial, vizinho});
  }

  while (!fila.empty()) {
    auto [peso, atual, vizinho] = fila.top();
    fila.pop();

    if(!visitado[vizinho]) {
      visitado[vizinho] = true;
      mst.push_back({atual, vizinho, peso});  // adiciona aresta na MST

      // adiciona as novas arestas do vértice vizinho
      for (auto [peso_vizinho, vizinhoVizinho] : grafo.listaAdjacencia[vizinho]) {
        if(!visitado[vizinhoVizinho]) {
          fila.push({peso_vizinho, vizinho, vizinhoVizinho});
        }
      }
    }
  }

  return mst;
}

int main() {
  Grafo g(7);

  g.adicionarAresta(0, 1, 7);
  g.adicionarAresta(0, 3, 5);
  g.adicionarAresta(1, 2, 8);
  g.adicionarAresta(1, 3, 9);
  g.adicionarAresta(1, 4, 7);
  g.adicionarAresta(2, 4, 5);
  g.adicionarAresta(3, 5, 6);
  g.adicionarAresta(3, 4, 15);
  g.adicionarAresta(4, 5, 8);
  g.adicionarAresta(4, 6, 9);
  g.adicionarAresta(5, 6, 11);

  g.imprimir();

  vector<tuple<int, int, int>> mst = prim(g, 0);

  cout << "Arestas da Árvore Geradora Mínima:\n";
  for (auto [u, v, peso] : mst) {
      cout << u << " - " << v << " (Peso: " << peso << ")\n";
  }
}