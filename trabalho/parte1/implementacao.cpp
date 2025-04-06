#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <set>
#include <limits>

using namespace std;

typedef vector<tuple<int, int, bool>> ListaAdjacencia; // tuple<destino, peso, required>

/*
1. Quantidade de vértices; ✅
2. Quantidade de arestas; ✅
3. Quantidade de arcos; ✅
4. Quantidade de vértices requeridos; ✅
5. Quantidade de arestas requeridas; ✅
6. Quantidade de arcos requeridos; ✅
7. Densidade do grafo (order strength);
8. Componentes conectados;
9. Grau mínimo dos vértices;
10. Grau máximo dos vértices;
11. Intermediação - A Intermediação de um nó mede a frequência com que ele aparece nos caminhos mais curtos entre outros nós. Não é necessário calcular outros caminhos mais curtos alternativos;
12. Caminho médio;
13. Diâmetro.
*/

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
    for(int i = 0; i < this->quantidadeVertices; i++) {
      this->matrizW[i] = new int[this->quantidadeVertices];
    }
  }

  void inicializaPred() {
    this->pred = new int*[this->quantidadeVertices];
    for(int i = 0; i < this->quantidadeVertices; i++) {
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
    for(int i = 0; i < this->quantidadeVertices; i++) {
      for(int j = 0; j < this->quantidadeVertices; j++) {
        if(this->matrizW[i][j] != numeric_limits<int>::max() && this->matrizW[i][j] != 0) {
          this->pred[i][j] = i;
        } else {
          this->pred[i][j] = -1;
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
    this->preencheMatrizW();
    this->imprimirMatrizW();

    this->preenchePred();
    this->imprimirPred();

    int** matriz = this->matrizW;

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
    cout << "\nMatriz W:" << endl;
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

  void imprimirPred() {
    cout << "\nPred:" << endl;
    for (int i = 0; i < this->quantidadeVertices; i++) {
      for (int j = 0; j < this->quantidadeVertices; j++) {
        if (this->pred[i][j] == -1) {
          cout << "- ";
        } else {
          cout << this->pred[i][j] << " ";
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

  grafo.adicionarArco(0, 1, 3, true);
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
  grafo.floydWarshall();



  return 0;
}
