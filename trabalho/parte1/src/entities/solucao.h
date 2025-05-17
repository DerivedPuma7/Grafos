#pragma once

#include <iostream>
#include <climits>
#include <algorithm>
#include "grafo.h"

using namespace std;

enum TipoServico { NO, ARESTA, ARCO };
struct Servico {
  TipoServico tipo;
  int from; // para vertices, é o id do vertice
  int to;
  int demanda;
  int custoServico;
  bool atendido = false;
};

struct Rota {
  vector<int> caminho;
  vector<Servico> servicosAtendidos;
  int custoTotal = 0;
};

class Solucao {
private:
  Grafo grafo;
  int capacidadeVeiculo;
  int verticeDeposito;
  vector<Rota> rotas;
  vector<Servico> servicosPendentes;
  vector<Rota> rotasSolucao;

  void identificarServicosPendentes() {
    this->identificarVerticesRequeridos();
    this->identificarArestasRequeridas();
    this->identificarArcosRequeridos();
  }

  void identificarVerticesRequeridos() {
    vector<Vertice> verticesRequeridos = this->grafo.getVerticesRequeridos();
    bool atendido = false;
    for(const auto& vertice : verticesRequeridos) {
      this->servicosPendentes.push_back({ NO, vertice.id, -1, vertice.demanda, vertice.custo, atendido });
    }
  }

  void identificarArestasRequeridas() {
    vector<Aresta> arestasRequeridas = this->grafo.getArestasRequeridas();
    bool atendido = false;
    for(const auto& aresta: arestasRequeridas) {
      this->servicosPendentes.push_back({ ARESTA, aresta.origem, aresta.destino, aresta.demanda, aresta.custoServico, atendido });
    }
  }

  void identificarArcosRequeridos() {
    vector<Aresta> arcosRequeridos = this->grafo.getArcosRequeridos();
    bool atendido = false;
    for(const auto& arco: arcosRequeridos) {
      this->servicosPendentes.push_back({ ARCO, arco.origem, arco.destino, arco.demanda, arco.custoServico, atendido });
    }
  }

  void imprimirServicosPendentes() {
    cout << "vertices: " << endl;
    for(const auto& servico : this->servicosPendentes) {
      if(servico.tipo == NO && !servico.atendido) {
        cout << servico.from << endl;
      }
    }
    cout << endl;

    cout << "arestas: " << endl;
    for(const auto& servico : this->servicosPendentes) {
      if(servico.tipo == ARESTA && !servico.atendido) {
        cout << servico.from << "->" << servico.to << endl;
      }
    }
    cout << endl;

    cout << "arcos: " << endl;
    for(const auto& servico : this->servicosPendentes) {
      if(servico.tipo == ARCO && !servico.atendido) {
        cout << servico.from << "->" << servico.to << endl;
      }
    }
    cout << endl;
  }

public:
  Solucao(const Grafo& grafo, int capacidadeVeiculo, int verticeDeposito)
  : grafo(grafo), capacidadeVeiculo(capacidadeVeiculo), verticeDeposito(verticeDeposito) {
    this->identificarServicosPendentes();
    // this->imprimirServicosPendentes();
    this->encontrarRotas();
    this->imprimirRotas();
  }

  void encontrarRotas() {
    while(this->aindaExisteServicoPendente()) {
      int cargaRestante = this->capacidadeVeiculo;
      int verticeAtual = this->verticeDeposito;
      Rota rotaAtual;
      rotaAtual.caminho.push_back(this->verticeDeposito);

      while(true) {
        tuple<int, int> melhorServico = this->encontrarMelhorServico(verticeAtual, cargaRestante); // melhorIndice, menorCusto
        auto [melhorIndice, menorCusto] = melhorServico;
        
        // não há alternativas de caminho, voltar ao deposito
        if(melhorIndice == -1) { 
          int custoAteDeposito = this->grafo.getCustoCaminhoMinimo(verticeAtual, this->verticeDeposito);
          rotaAtual.custoTotal += custoAteDeposito;
          rotaAtual.caminho.push_back(this->verticeDeposito);
          this->rotasSolucao.push_back(rotaAtual);
          break;
        }

        Servico& servico = this->servicosPendentes[melhorIndice];
        this->atenderServico(servico, rotaAtual, cargaRestante, verticeAtual, menorCusto);
      }
    }
  }

  bool aindaExisteServicoPendente() {
    bool existePendencia = false;
    for(int i = 0; i < this->servicosPendentes.size(); i++) {
      if(this->servicosPendentes[i].atendido == false) {
        existePendencia = true;
        break;
      }
    }
    return existePendencia;
  }

  tuple<int, int> encontrarMelhorServico(int verticeAtual, int cargaRestante) {
    int melhorIndice = -1;
    int menorCusto = INT_MAX;

    for(int i = 0; i < this->servicosPendentes.size(); i++) {
      Servico s = this->servicosPendentes[i];
      if(s.atendido || s.demanda > cargaRestante) continue;

      int destino = s.from;
      int custoAteServico = this->grafo.getCustoCaminhoMinimo(verticeAtual, destino);
      if(custoAteServico < menorCusto) {
        melhorIndice = i;
        menorCusto = custoAteServico;
      }
    }
    return { melhorIndice, menorCusto };
  }

  void atenderServico(Servico& servico, Rota& rotaAtual, int& cargaRestante, int& verticeAtual, int menorCusto) {
    servico.atendido = true;
    
    int destino = servico.tipo == NO ? servico.from : servico.to;
    
    rotaAtual.custoTotal += menorCusto + servico.custoServico;
    rotaAtual.servicosAtendidos.push_back(servico);

    rotaAtual.caminho.push_back(destino);
    cargaRestante -= servico.demanda;
    verticeAtual = destino;
  }

  void imprimirRotas() {
    cout << "Rotas: " << endl;
    for(int i = 0; i < this->rotasSolucao.size(); i++) {
      cout << "Rota[" << i << "]" << endl;
      for(int j = 0; j < this->rotasSolucao[i].caminho.size(); j++) {
        cout << "\t caminho: " << this->rotasSolucao[i].caminho[j] << endl;
      }
    }
  }
};
