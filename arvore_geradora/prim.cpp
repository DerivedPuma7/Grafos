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

  // implementação para um grafo nao direcionado
  void adicionarAresta(int u, int v, int peso) { // vertice u------peso------vertice v
    listaAdjacencia[u].push_back({peso, v});
    listaAdjacencia[v].push_back({peso, u});
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

}