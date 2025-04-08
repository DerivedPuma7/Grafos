#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <set>
#include <limits>

using namespace std;

typedef vector<tuple<int, int, int, bool>> ListaAdjacencia; // tuple<destino, peso, demanda, required>
typedef pair<int**, int**> WAndPred; // pair<W, pred>

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
      for(auto [destino, peso, demanda, required] : listaAdjacencia[i]) {
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

  double calculaDensidadeGrafoDirecionado() {
    double densidade = (double)this->quantidadeArestas / (this->quantidadeVertices * (this->quantidadeVertices - 1));
    return densidade;
  }

  double calculaDensidadeGrafoNaoDirecionado() {
    double densidade = (double)this->quantidadeArestas / (this->quantidadeVertices * (this->quantidadeVertices - 1) / 2);
    return densidade;
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

  void adicionarAresta(int verticeOrigem, int verticeDestino, int peso, int demanda, bool required) {
    this->quantidadeArestas++;
    if(required) {
      this->quantidadeArestasRequeridas++;
    }
    this->listaAdjacencia[verticeOrigem].push_back({verticeDestino, peso, demanda, required});
    this->listaAdjacencia[verticeDestino].push_back({verticeOrigem, peso, demanda, required});
  }

  void adicionarArco(int verticeOrigem, int verticeDestino, int peso, int demanda, bool required) {
    this->quantidadeArcos++;
    if(required) {
      this->quantidadeArcosRequeridos++;
    }
    this->listaAdjacencia[verticeOrigem].push_back({verticeDestino, peso, demanda, required});
  }

  void adicionarVerticeRequerido(int vertice) {
    this->verticesRequeridos.insert(vertice);
  }

  void floydWarshall() {
    this->preencheMatrizW();
    this->preenchePred();

    for(int k = 0; k < this->quantidadeVertices; k++) {
      for(int i = 0; i < this->quantidadeVertices; i++) {
        for(int j = 0; j < this->quantidadeVertices; j++) {
          if(
            this->matrizW[i][k] != numeric_limits<int>::max() && 
            this->matrizW[k][j] != numeric_limits<int>::max() &&
            this->matrizW[i][j] > this->matrizW[i][k] + this->matrizW[k][j]
          ) {
            this->matrizW[i][j] = this->matrizW[i][k] + this->matrizW[k][j];
            this->pred[i][j] = this->pred[k][j];
          }
        }
      }
    }
  }

  void imprimirGrafo() {
    for (int i = 0; i < this->quantidadeVertices; i++) {
      cout << i << " → ";
      for (auto [destino, peso, demanda, required] : listaAdjacencia[i]) {
        string tipo = required ? "Required" : "Not Required";
        cout << "{" << destino << ", " << peso << ", " << tipo  << "} ";
      }
      cout << endl;
    }
    cout << endl;
  }

  void imprimirMatrizW(int** matriz) {
    cout << "\nMatriz W:" << endl;
    for (int i = 0; i < this->quantidadeVertices; i++) {
      for (int j = 0; j < this->quantidadeVertices; j++) {
        if (matriz[i][j] == numeric_limits<int>::max()) {
          cout << "INF ";
        } else {
          cout << matriz[i][j] << " ";
        }
      }
      cout << endl;
    }
    cout << endl;
  }

  void imprimirPred(int** matriz) {
    cout << "\nPred:" << endl;
    for (int i = 0; i < this->quantidadeVertices; i++) {
      for (int j = 0; j < this->quantidadeVertices; j++) {
        if (matriz[i][j] == -1) {
          cout << "- ";
        } else {
          cout << matriz[i][j] << " ";
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

  // pair<W, pred>
  WAndPred getWAndPred() {
    return {this->matrizW, this->pred};
  }

  double getDensidadeGrafo() {
    if(this->quantidadeArestas == 0 && this->quantidadeArcos == 0) {
      return 0;
    }
    if(this->quantidadeArcos == 0) {
      return this->calculaDensidadeGrafoNaoDirecionado();
    }
    return this->calculaDensidadeGrafoDirecionado();
  }

  int getGrauMinSaida() {
    int grauMin = numeric_limits<int>::max();
    for(int i = 0; i < this->quantidadeVertices; i++) {
      int grau = this->listaAdjacencia[i].size();
      grauMin = min(grauMin, grau);
    }
    return grauMin;
  }

  int getGrauMaxSaida() {
    int grauMax = 0;
    for(int i = 0; i < this->quantidadeVertices; i++) {
      int grau = this->listaAdjacencia[i].size();
      grauMax = max(grauMax, grau);
    }
    return grauMax;
  }

  int getGrauMinEntrada() {
    int grauMin = numeric_limits<int>::max();
    for(int i = 0; i < this->quantidadeVertices; i++) {

      int grauEntradaI = 0;
      for(int j = 0; j < this->quantidadeVertices; j++) {
        if(j == i) {
          continue;
        }
        for(auto [destino, peso, demanda, required] : listaAdjacencia[j]) {
          if(destino == i) {
            grauEntradaI++;
          }
        }
      }
      grauMin = min(grauMin, grauEntradaI);
    }
    return grauMin;
  }

  int getGrauMaxEntrada() {
    int grauMax = 0;
    for(int i = 0; i < this->quantidadeVertices; i++) {

      int grauEntradaI = 0;
      for(int j = 0; j < this->quantidadeVertices; j++) {
        if(j == i) {
          continue;
        }
        
        for(auto [destino, peso, demanda, required] : listaAdjacencia[j]) {
          if(destino == i) {
            grauEntradaI++;
          }
        }
      }
      grauMax = max(grauMax, grauEntradaI);
    }
    return grauMax;
  }

  /*
    BFS = busca em largura
    Um grafo conexo possui apenas 1 componente conectado
    Um grafo com múltiplos componentes conectados é um grafo desconexo
  */
  int getComponentesConectados() {
    vector<bool> visitado(this->quantidadeVertices, false);
    int componentes = 0;

    for(int i = 0; i < this->quantidadeVertices; i++) {
      if(!visitado[i]) {
        componentes++;
        queue<int> fila;
        fila.push(i);
        visitado[i] = true;

        while(!fila.empty()) {
          int atual = fila.front();
          fila.pop();

          for(auto [vizinho, peso, demanda, required] : this->listaAdjacencia[atual]) {
            if (!visitado[vizinho]) {
              visitado[vizinho] = true;
              fila.push(vizinho);
            }
          }
        }
      }
    }

    return componentes;
  }

  /**
   * intermediação representa os pontos principais na malha de caminhos
   * nós com alta intermediação são cruciais
   */
  vector<double> getIntermediacao() {
    vector<double> intermediacao(this->quantidadeVertices, 0.0);
    
    for(int s = 0; s < this->quantidadeVertices; s++) {
      for(int t = 0; t < this->quantidadeVertices; t++) {
        if(s == t) continue;
        
        // Reconstruir o caminho mais curto de s para t
        vector<int> caminho;
        int atual = t;
        while(atual != s && atual != -1) {
          caminho.push_back(atual);
          atual = this->pred[s][atual];
        }
        if(atual == -1) continue; // não há caminho
        caminho.push_back(s);
        
        // Contar os nós intermediários (excluindo s e t)
        for(int v : caminho) {
          if(v != s && v != t) {
            intermediacao[v] += 1.0;
          }
        }
      }
    }
    
    // normalizar (opcional, dependendo da definição)
    int pares = this->quantidadeVertices * (this->quantidadeVertices - 1);
    if(pares > 0) {
      for(int i = 0; i < this->quantidadeVertices; i++) {
        intermediacao[i] /= pares;
      }
    }
    
    return intermediacao;
  }

  /**
   * representa a média das distâncias dos caminhos mais curtos entre todos os pares de nós conectados no grafo
   */
  double getCaminhoMedio() {
    double soma = 0.0;
    int contagem = 0;
    
    for(int i = 0; i < this->quantidadeVertices; i++) {
      for(int j = 0; j < this->quantidadeVertices; j++) {
        if(i != j && this->matrizW[i][j] != numeric_limits<int>::max()) {
          soma += this->matrizW[i][j];
          contagem++;
        }
      }
    }
    
    return contagem > 0 ? soma / contagem : 0.0;
  }

  /**
   * representa o maior caminho mais curto entre qualquer par de nós no grafo
   * grafos com diâmetro grande podem precisar de múltiplos depósitos
   */
  int calcularDiametro() {
    int diametro = 0;
    
    for(int i = 0; i < this->quantidadeVertices; i++) {
      for(int j = 0; j < this->quantidadeVertices; j++) {
        if(i != j && this->matrizW[i][j] != numeric_limits<int>::max()) {
          diametro = max(diametro, this->matrizW[i][j]);
        }
      }
    }
    return diametro;
  }
};

