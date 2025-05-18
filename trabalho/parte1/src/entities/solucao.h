#pragma once

#include <iostream>
#include <climits>
#include <algorithm>
#include "grafo.h"

using namespace std;

enum TipoServico { NO, ARESTA, ARCO };
struct Servico {
  TipoServico tipo;
  string id;
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
      string idServico = "NO_" + to_string(vertice.id);
      this->servicosPendentes.push_back({ NO, idServico, vertice.id, -1, vertice.demanda, vertice.custo, atendido });
    }
  }

  void identificarArestasRequeridas() {
    vector<Aresta> arestasRequeridas = this->grafo.getArestasRequeridas();
    bool atendido = false;
    for(const auto& aresta: arestasRequeridas) {
      string idServico = "ARESTA_" + to_string(aresta.origem); + "_" + to_string(aresta.destino);
      this->servicosPendentes.push_back({ ARESTA, idServico, aresta.origem, aresta.destino, aresta.demanda, aresta.custoServico, atendido });
    }
  }

  void identificarArcosRequeridos() {
    vector<Aresta> arcosRequeridos = this->grafo.getArcosRequeridos();
    bool atendido = false;
    for(const auto& arco: arcosRequeridos) {
      string idServico = "ARCO_" + to_string(arco.origem); + "_" + to_string(arco.destino);
      this->servicosPendentes.push_back({ ARCO, idServico, arco.origem, arco.destino, arco.demanda, arco.custoServico, atendido });
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

  void print(string param, int value) {
    cout << param << value << endl;
  }
  void br() {
    cout << "\n";
  }

  void encontrarRotas() {
    int iteracao = 1;
    while(this->aindaExisteServicoPendente()) {
      this->print("iteração: ", iteracao);

      int cargaRestante = this->capacidadeVeiculo;
      int verticeAtual = this->verticeDeposito;

      this->print("cargaRestante: ", cargaRestante);
      this->print("verticeAtual: ", verticeAtual);

      Rota rotaAtual;
      rotaAtual.caminho.push_back(this->verticeDeposito);

      while(true) {

        tuple<int, int> melhorServico = this->encontrarMelhorServico(verticeAtual, cargaRestante);
        auto [melhorIndice, menorCusto] = melhorServico;
        
        // this->print("\t melhorIndice: ", melhorIndice);
        this->print("\t custo ate o serviço: ", menorCusto);
        
        // não há alternativas de caminho, voltar ao deposito
        if(melhorIndice == -1) { 
          cout << "\t\t voltando ao deposito" << endl;
          this->print("\t\t vertice atual: ", verticeAtual);

          int custoAteDeposito = this->grafo.getCustoCaminhoMinimo(verticeAtual, this->verticeDeposito);
          rotaAtual.custoTotal += custoAteDeposito;
          rotaAtual.caminho.push_back(this->verticeDeposito);
          this->rotasSolucao.push_back(rotaAtual);
          break;
        }

        Servico& servico = this->servicosPendentes[melhorIndice];

        cout << "\t realizando serviços" << endl;
        string tipoServico = servico.tipo == NO ? "VERTICE" : servico.tipo == ARESTA ? "ARESTA" : "ARCO";
        cout << "\t servico a ser atendido: " << endl;
        cout << "\t tipo: " << tipoServico << " | from: " << servico.from << " | to: " << servico.to << endl;
        
        this->atenderServico(servico, rotaAtual, cargaRestante, verticeAtual, menorCusto);
        
        this->br();
      }
      iteracao++;
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
    Servico melhorServico;

    print("\t\tencontrando melhor serviço. vertice atual: ", verticeAtual);

    for(int i = 0; i < this->servicosPendentes.size(); i++) {
      Servico s = this->servicosPendentes[i];
      if(s.atendido || s.demanda > cargaRestante) continue;

      int destino = s.from;
      if(verticeAtual == s.from && s.to != -1) {
        destino = s.to;
      }
      
      // print("\t\tencontrando melhor serviço. destino: ", destino);
      
      int custoAteServico = this->grafo.getCustoCaminhoMinimo(verticeAtual, destino);
      // print("\t\t encontrando melhor serviço. destino: ", destino);
      // print("\t\t encontrando melhor serviço. custo: ", custoAteServico);

      if(
        (custoAteServico < menorCusto) ||
        (custoAteServico == menorCusto && s.tipo != NO)
      ) {
        melhorServico = s;
        melhorIndice = i;
        menorCusto = custoAteServico;
      }
    }
    cout << "\t\t melhor serviço encontrado. origem: " << melhorServico.from << " destino: " << melhorServico.to << endl;
    this->print("\t\t custo ate o proximo serviço: ", menorCusto);

    Servico *servicoEmVerticeAssociadoAProximaOrigem = this->getServicoPendenteAssociadoAoVertice(melhorServico.from);

    if(
      (melhorServico.tipo == ARESTA || melhorServico.tipo == ARCO)  && 
      servicoEmVerticeAssociadoAProximaOrigem != NULL
    ) {
      for(int i = 0; i < this->servicosPendentes.size(); i++) {
        if(servicoEmVerticeAssociadoAProximaOrigem->id == this->servicosPendentes[i].id) {
          melhorIndice = i;
        }
      }
    }

    return { melhorIndice, menorCusto };
  }

  void atenderServico(Servico& servico, Rota& rotaAtual, int& cargaRestante, int& verticeAtual, int menorCusto) {
    servico.atendido = true;

    int destino = servico.tipo == NO ? servico.from : servico.to;

    this->print("\t vertice atual: ", verticeAtual);
    this->print("\t proximo destino: ", destino);
    
    rotaAtual.custoTotal += menorCusto + servico.custoServico;
    rotaAtual.servicosAtendidos.push_back(servico);

    rotaAtual.caminho.push_back(destino);
    cargaRestante -= servico.demanda;
    verticeAtual = destino;
  }

  Servico* getServicoPendenteAssociadoAoVertice(int vertice) {
    for(int i = 0; i < this->servicosPendentes.size(); i++) {
      if(
        this->servicosPendentes[i].tipo == NO &&
        this->servicosPendentes[i].atendido == false &&
        this->servicosPendentes[i].from == vertice
      ) {
        return &this->servicosPendentes[i];
      }
    }
    return NULL;
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


/**
 * TODO:
 * ler a demanda e o nó deposito de cada arquivo
 * ler todos os arquivos
 * saída de acordo com o esperado
 * leitura dos dados via linha de comando.
 */