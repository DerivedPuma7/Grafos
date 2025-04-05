#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <set>
#include <limits>

using namespace std;

typedef vector<tuple<int, int, bool>> ListaAdjacencia; // tuple<destino, peso, required>

class Grafo  {
private:
  int quantidadeVertices;
  int quantidadeArestas;
  int quantidadeArestasRequeridas;
  int quantidadeArcos;
  int quantidadeArcosRequeridos;
  ListaAdjacencia* listaAdjacencia;
  set<int> verticesRequeridos;
  int** matrizW;
  int** pred;

  void inicializaMatrizW() {
    this->matrizW = new int*[this->quantidadeVertices];
    for (int i = 0; i < this->quantidadeVertices; i++) {
      this->matrizW[i] = new int[this->quantidadeVertices];
    }
  }

  void inicializaPred() {
    this->pred = new int*[this->quantidadeVertices];
    for (int i = 0; i < this->quantidadeVertices; i++) {
      this->pred[i] = new int[this->quantidadeVertices];
    }
  }

  void preencheMatrizW() {
    for(int i = 0; i < this->quantidadeVertices; i++) {
      for(int j = 0; j < this->quantidadeVertices; j++) {
        if(i == j) {
          this->matrizW[i][j] = 0;
        } else {
          this->matrizW[i][j] = numeric_limits<int>::max();
        }
      }
    }
    for(int i = 0; i < this->quantidadeVertices; i++) {
      for(auto [destino, peso, required] : listaAdjacencia[i]) {
        this->matrizW[i][destino] = peso;
      }
    }
  }

  void preenchePred() {
    for (int i = 0; i < this->quantidadeVertices; i++) {
      for (int j = 0; j < this->quantidadeVertices; j++) {
        if(this->matrizW[i][j] != numeric_limits<int>::max() && this->matrizW[i][j] != 0) {
          this->pred[i][j] = i;
        } else {
          this->pred[i][j] = numeric_limits<int>::max();
        }
      }
    }
  }

public:
  Grafo(int quantidadeVertices) {
    this->quantidadeVertices = quantidadeVertices;
    this->quantidadeArestas = 0;
    this->quantidadeArestasRequeridas = 0;
    this->quantidadeArcos = 0;
    this->quantidadeArcosRequeridos = 0;
    this->verticesRequeridos = set<int>();
    this->listaAdjacencia = new ListaAdjacencia[quantidadeVertices];
    this->inicializaMatrizW();
    this->inicializaPred();
  }

  void adicionarAresta(int verticeOrigem, int verticeDestino, int peso, bool required) {
    this->quantidadeArestas++;
    if(required) {
      this->quantidadeArestasRequeridas++;
    }
    this->listaAdjacencia[verticeOrigem].push_back({verticeDestino, peso, required});
    this->listaAdjacencia[verticeDestino].push_back({verticeOrigem, peso, required});
  }

  void adicionarArco(int verticeOrigem, int vesticeDestino, int peso, bool required) {
    this->quantidadeArcos++;
    if(required) {
      this->quantidadeArcosRequeridos++;
    }
    this->listaAdjacencia[verticeOrigem].push_back({vesticeDestino, peso, required});
  }

  void adicionarVerticeRequerido(int vertice) {
    this->verticesRequeridos.insert(vertice);
  }

  int** floydWarshall() {
    int** matriz = this->matrizW;
    preencheMatrizW();
    preenchePred();

    for(int k = 0; k < this->quantidadeVertices; k++) {
      for(int i = 0; i < this->quantidadeVertices; i++) {
        for(int j = 0; j < this->quantidadeVertices; j++) {
          if(
            matriz[i][k] != numeric_limits<int>::max() && 
            matriz[k][j] != numeric_limits<int>::max() &&
            matriz[i][j] > matriz[i][k] + matriz[k][j]
          ) {
            matriz[i][j] = matriz[i][k] + matriz[k][j];
            pred[i][j] = pred[k][j];
          }
        }
      }
    }
    return matriz;
  }

  void imprimirGrafo() {
    for (int i = 0; i < this->quantidadeVertices; i++) {
      cout << i << " → ";
      for (auto [destino, peso, required] : listaAdjacencia[i]) {
        string tipo = required ? "Required" : "Not Required";
        cout << "{" << destino << ", " << peso << ", " << tipo  << "} ";
      }
      cout << endl;
    }
    cout << endl;
  }

  void imprimirMatrizW() {
    cout << "\nMatriz W:\n";
    for (int i = 0; i < this->quantidadeVertices; i++) {
      for (int j = 0; j < this->quantidadeVertices; j++) {
        if (this->matrizW[i][j] == numeric_limits<int>::max()) {
          cout << "INF ";
        } else {
          cout << this->matrizW[i][j] << " ";
        }
      }
      cout << endl;
    }
    cout << endl;
  }

  int getQuantidadeVertices() {
    return this->quantidadeVertices;
  }

  int getQuantidadeArestas() {
    return this->quantidadeArestas;
  }

  int getQuantidadeArcos() {
    return this->quantidadeArcos;
  }

  int getQuantidadeArestasRequeridas() {
    return this->quantidadeArestasRequeridas;
  }

  int getQuantidadeArcosRequeridos() {
    return this->quantidadeArcosRequeridos;
  }

  int getQuantidadeVerticesRequeridos() {
    return this->verticesRequeridos.size();
  }
};

int main() {
  Grafo grafo(5);

  grafo.adicionarAresta(0, 1, 3, true);
  grafo.adicionarArco(0, 2, 8, true);
  grafo.adicionarArco(0, 4, -4, true);

  grafo.adicionarArco(1, 4, 7, false);
  grafo.adicionarArco(1, 3, 1, true);

  grafo.adicionarArco(2, 1, 4, false);

  grafo.adicionarArco(3, 2, -5, false);
  grafo.adicionarArco(3, 0, 2, false);

  grafo.adicionarArco(4, 3, 6, false);

  grafo.adicionarVerticeRequerido(0);

  grafo.imprimirGrafo();
  grafo.imprimirMatrizW();

  cout << "Quantidade de vertices: " << grafo.getQuantidadeVertices() << endl;
  cout << "Quantidade de arestas: " << grafo.getQuantidadeArestas() << endl;
  cout << "Quantidade de arcos: " << grafo.getQuantidadeArcos() << endl << endl;

  cout << "Quantidade de vertices requeridas: " << grafo.getQuantidadeVerticesRequeridos() << endl;
  cout << "Quantidade de arestas requeridas: " << grafo.getQuantidadeArestasRequeridas() << endl;
  cout << "Quantidade de arcos requeridas: " << grafo.getQuantidadeArcosRequeridos() << endl << endl;

  return 0;
}
