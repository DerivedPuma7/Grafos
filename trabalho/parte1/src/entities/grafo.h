#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <set>
#include <limits>
#include <string>
#include "vertice.h"
#include "aresta.h"

using namespace std;

typedef vector<tuple<int, int, int, int, bool>> ListaAdjacencia; // tuple<destino, peso, demanda, custoServico, required>
typedef pair<int**, int**> WAndPred; // pair<W, pred>

class Grafo  {
private:
  string nome;
  int quantidadeVertices;
  int quantidadeArestas;
  int quantidadeArestasRequeridas;
  int quantidadeArcos;
  int quantidadeArcosRequeridos;
  ListaAdjacencia* listaAdjacencia;
  set<Vertice> verticesRequeridos;
  set<Aresta> arestasRequeridas;
  set<Aresta> arcosRequeridos;
  int** matrizW;
  int** pred;

  void inicializaMatrizW() {
    this->matrizW = new int*[this->quantidadeVertices+1];
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      this->matrizW[i] = new int[this->quantidadeVertices+1];
    }
  }

  void inicializaPred() {
    this->pred = new int*[this->quantidadeVertices+1];
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      this->pred[i] = new int[this->quantidadeVertices+1];
    }
  }

  void preencheMatrizW() {
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      for(int j = 1; j <= this->quantidadeVertices; j++) {
        if(i == j) {
          this->matrizW[i][j] = 0;
        } else {
          this->matrizW[i][j] = numeric_limits<int>::max();
        }
      }
    }
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      for(auto [destino, peso, demanda, custoServico, required] : listaAdjacencia[i]) {
        this->matrizW[i][destino] = peso;
      }
    }
  }

  void preenchePred() {
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      for(int j = 1; j <= this->quantidadeVertices; j++) {
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
  Grafo(int quantidadeVertices, string nome) {
    this->quantidadeVertices = quantidadeVertices;
    this->nome = nome;
    this->quantidadeArestas = 0;
    this->quantidadeArestasRequeridas = 0;
    this->quantidadeArcos = 0;
    this->quantidadeArcosRequeridos = 0;
    this->verticesRequeridos = set<Vertice>();
    this->listaAdjacencia = new ListaAdjacencia[quantidadeVertices+1];
    this->inicializaMatrizW();
    this->inicializaPred();
  }

  void adicionarAresta(int verticeOrigem, int verticeDestino, int peso, int demanda, int custoServico, bool required, int idAlternativo) {
    this->quantidadeArestas++;
    if(required) {
      this->quantidadeArestasRequeridas++;
      string id = to_string(verticeOrigem) + "_" + to_string(verticeDestino);
      Aresta aresta(id, verticeOrigem, verticeDestino, peso, demanda, custoServico, idAlternativo);
      this->arestasRequeridas.insert(aresta);
    }
    this->listaAdjacencia[verticeOrigem].push_back({verticeDestino, peso, demanda, custoServico, required});
    this->listaAdjacencia[verticeDestino].push_back({verticeOrigem, peso, demanda, custoServico, required});
  }

  void adicionarArco(int verticeOrigem, int verticeDestino, int peso, int demanda, int custoServico, bool required, int idAlternativo) {
    this->quantidadeArcos++;
    if(required) {
      this->quantidadeArcosRequeridos++;
      string id = to_string(verticeOrigem) + "_" + to_string(verticeDestino);
      Aresta aresta(id, verticeOrigem, verticeDestino, peso, demanda, custoServico, idAlternativo);
      this->arcosRequeridos.insert(aresta);
    }
    this->listaAdjacencia[verticeOrigem].push_back({verticeDestino, peso, demanda, custoServico, required});
  }

  void adicionarVerticeRequerido(int vertice, int demanda, int custo, int idAlternativo) {
    Vertice verticeRequerido(vertice, demanda, custo, idAlternativo);
    this->verticesRequeridos.insert(verticeRequerido);
  }

  void floydWarshall() {
    this->preencheMatrizW();
    this->preenchePred();

    for(int k = 1; k <= this->quantidadeVertices; k++) {
      for(int i = 1; i <= this->quantidadeVertices; i++) {
        for(int j = 1; j <= this->quantidadeVertices; j++) {
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

    this->imprimirMatrizW(this->getWAndPred().first);
    cout << "\n\n";
  }

  void imprimirGrafo() {
    for (int i = 1; i <= this->quantidadeVertices; i++) {
      cout << i << " → ";
      for (auto [destino, peso, demanda, custoServico, required] : listaAdjacencia[i]) {
        string tipo = required ? "Required" : "Not Required";
        cout << "{" << destino << ", " << peso << ", " << tipo  << "} ";
      }
      cout << endl;
    }
    cout << endl;
  }

  void imprimirMatrizW(int** matriz) {
    cout << "\nMatriz W:" << endl;
    for (int i = 1; i <= this->quantidadeVertices; i++) {
      for (int j = 1; j <= this->quantidadeVertices; j++) {
        if (matriz[i][j] == numeric_limits<int>::max()) {
          cout << "INF     ";
        } else {
          cout << matriz[i][j] << "     ";
        }
      }
      cout << endl;
    }
    cout << endl;
  }

  void imprimirPred(int** matriz) {
    cout << "\nPred:" << endl;
    for (int i = 1; i <= this->quantidadeVertices; i++) {
      for (int j = 1; j <= this->quantidadeVertices; j++) {
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

  string getNome() {
    return this->nome;
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
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      int grau = this->listaAdjacencia[i].size();
      grauMin = min(grauMin, grau);
    }
    return grauMin;
  }

  int getGrauMaxSaida() {
    int grauMax = 0;
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      int grau = this->listaAdjacencia[i].size();
      grauMax = max(grauMax, grau);
    }
    return grauMax;
  }

  int getGrauMinEntrada() {
    int grauMin = numeric_limits<int>::max();
    for(int i = 1; i <= this->quantidadeVertices; i++) {

      int grauEntradaI = 0;
      for(int j = 1; j <= this->quantidadeVertices; j++) {
        if(j == i) {
          continue;
        }
        for(auto [destino, peso, demanda, custoServico, required] : listaAdjacencia[j]) {
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
    for(int i = 1; i <= this->quantidadeVertices; i++) {

      int grauEntradaI = 0;
      for(int j = 1; j <= this->quantidadeVertices; j++) {
        if(j == i) {
          continue;
        }
        
        for(auto [destino, peso, demanda, custoServico, required] : listaAdjacencia[j]) {
          if(destino == i) {
            grauEntradaI++;
          }
        }
      }
      grauMax = max(grauMax, grauEntradaI);
    }
    return grauMax;
  }

  /**
   * intermediação representa os pontos principais na malha de caminhos
   * nós com alta intermediação são cruciais
   */
  vector<double> getIntermediacao() {
    vector<double> intermediacao(this->quantidadeVertices+1, 0.0);
    
    for(int s = 1; s <= this->quantidadeVertices; s++) {
      for(int t = 1; t <= this->quantidadeVertices; t++) {
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
      for(int i = 1; i <= this->quantidadeVertices; i++) {
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
    
    for(int i = 1; i <= this->quantidadeVertices; i++) {
      for(int j = 1; j <= this->quantidadeVertices; j++) {
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
  int getDiametro() {
    int diametro = 0;
    
    for(int i = 1; i < this->quantidadeVertices; i++) {
      for(int j = 1; j < this->quantidadeVertices; j++) {
        if(i != j && this->matrizW[i][j] != numeric_limits<int>::max()) {
          diametro = max(diametro, this->matrizW[i][j]);
        }
      }
    }
    return diametro;
  }

  vector<Vertice> getVerticesRequeridos() {
    vector<Vertice> verticesRequeridos;
    for(auto vertice : this->verticesRequeridos) {
      verticesRequeridos.push_back(vertice);
    }

    return verticesRequeridos;
  }

  vector<Aresta> getArestasRequeridas() {
    vector<Aresta> arestasRequeridas;
    for(auto aresta : this->arestasRequeridas) {
      arestasRequeridas.push_back(aresta);
    }

    return arestasRequeridas;
  }

  vector<Aresta> getArcosRequeridos() {
    vector<Aresta> arcosRequeridos;
    for(auto arco : this->arcosRequeridos) {
      arcosRequeridos.push_back(arco);
    }

    return arcosRequeridos;
  }

  void imprimirVerticesRequeridos() {
    cout << "Vértices Requeridos grafo: " << endl;
    for(auto vertice : this->verticesRequeridos) {
      cout << vertice.id << " ";
    }
    cout << endl << endl;
  }

  void imprimirArestasRequeridas() {
    cout << "Arestas Requeridas: " << endl;
    for(auto aresta : this->arestasRequeridas) {
      cout << aresta.origem << " -> " << aresta.destino << endl;
    }
    cout << endl;
  }

  void imprimirArcosRequeridos() {
    cout << "Arcos Requeridos: " << endl;
    for(auto arco : this->arcosRequeridos) {
      cout << arco.origem << " -> " << arco.destino << endl;
    }
    cout << endl;
  }

  int getCustoCaminhoMinimo(int origem, int destino) {
    WAndPred wAndPred = this->getWAndPred();
    int** matrizW = wAndPred.first;
    return matrizW[origem][destino];
  }
};

