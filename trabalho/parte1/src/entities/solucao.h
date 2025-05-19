#pragma once

#include <iomanip> 
#include <iostream>
#include <climits>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include "grafo.h"
#include "servico.h"
#include "rota.h"

using namespace std;

class Solucao {
private:
  Grafo grafo;
  int capacidadeVeiculo;
  int verticeDeposito;
  int custoTotal = 0;
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
      this->servicosPendentes.push_back({ NO, to_string(vertice.idAlternativo), vertice.id, -1, vertice.demanda, vertice.custo, atendido });
    }
  }

  void identificarArestasRequeridas() {
    vector<Aresta> arestasRequeridas = this->grafo.getArestasRequeridas();
    bool atendido = false;
    for(const auto& aresta: arestasRequeridas) {
      string idServico = "ARESTA_" + to_string(aresta.origem) + "_" + to_string(aresta.destino);
      this->servicosPendentes.push_back({ ARESTA, to_string(aresta.idAlternativo), aresta.origem, aresta.destino, aresta.demanda, aresta.custoServico, atendido });
    }
  }

  void identificarArcosRequeridos() {
    vector<Aresta> arcosRequeridos = this->grafo.getArcosRequeridos();
    bool atendido = false;
    for(const auto& arco: arcosRequeridos) {
      this->servicosPendentes.push_back({ ARCO, to_string(arco.idAlternativo), arco.origem, arco.destino, arco.demanda, arco.custoServico, atendido });
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
    clock_t inicio = clock();

    this->identificarServicosPendentes();
    this->encontrarRotas();

    clock_t final = clock();

    int clocksUsados = (final - inicio);

    this->imprimirRotas(clocksUsados);
  }

  void encontrarRotas() {
    
    while(this->aindaExisteServicoPendente()) {
      int cargaRestante = this->capacidadeVeiculo;
      int verticeAtual = this->verticeDeposito;

      Rota rotaAtual;
      rotaAtual.caminho.push_back(this->verticeDeposito);

      ServicoPrestadoDto servicoPrestadoDto(TipoServicoPrestado::D, "0", this->verticeDeposito, this->verticeDeposito);

      rotaAtual.servicosPrestados.push_back({servicoPrestadoDto});

      while(true) {

        tuple<int, int> melhorServico = this->encontrarMelhorServico(verticeAtual, cargaRestante);
        auto [melhorIndice, menorCusto] = melhorServico;
        
        // não há alternativas de caminho, voltar ao deposito
        if(melhorIndice == -1) { 

          ServicoPrestadoDto servicoPrestadoDto(TipoServicoPrestado::D, "0", this->verticeDeposito, this->verticeDeposito);

          rotaAtual.servicosPrestados.push_back({servicoPrestadoDto});

          int custoAteDeposito = this->grafo.getCustoCaminhoMinimo(verticeAtual, this->verticeDeposito);
          rotaAtual.custoTotal += custoAteDeposito;
          rotaAtual.caminho.push_back(this->verticeDeposito);
          this->rotasSolucao.push_back(rotaAtual);
          this->custoTotal += rotaAtual.custoTotal;
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

  /**
   * TODO
   * precisamos melhorar esses IFs para deixar mais claro os seus motivos
   */
  tuple<int, int> encontrarMelhorServico(int verticeAtual, int cargaRestante) {
    int melhorIndice = -1;
    int menorCusto = INT_MAX;
    Servico melhorServico;

    for(int i = 0; i < this->servicosPendentes.size(); i++) {
      Servico s = this->servicosPendentes[i];
      if(s.atendido || s.demanda > cargaRestante) continue;

      int destino = s.from;
      if(verticeAtual == s.from && s.tipo != NO) {
        destino = s.to;
      }

      int custoAteServico = this->grafo.getCustoCaminhoMinimo(verticeAtual, destino);

      // priorizando serviços em arcos e arestas
      if(
        (custoAteServico < menorCusto) ||
        (custoAteServico == menorCusto && s.tipo != NO)
      ) {
        melhorServico = s;
        melhorIndice = i;
        menorCusto = custoAteServico;
      }
    }

    Servico *servicoEmVerticeAssociadoAProximaOrigem = this->getServicoPendenteAssociadoAoVertice(melhorServico.from);

    // se o proximo serviço for um arco ou aresta obrigatorio, e houver um serviço associado o vertice de origem desse proximo serviço, execute o serviço do vertice primeiro
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

    ServicoPrestadoDto servicoPrestadoDto(TipoServicoPrestado::S, servico.id, verticeAtual, destino);

    rotaAtual.servicosPrestados.push_back({servicoPrestadoDto});
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

  void criarDiretorioResultado() {
    #ifdef _WIN32
      system("mkdir resultados >nul 2>&1");  // Windows (sem mostrar erro se já existir)
    #else
      system("mkdir -p resultados");         // Linux/macOS
    #endif
  }

  void imprimirRotas(int clocksUsados) {
    this->criarDiretorioResultado();

    string nomeArquivo = "resultados/sol-" + this->grafo.getNome() + ".dat";
    std::ofstream out(nomeArquivo);

    if (!out.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return;
    }

    out << this->custoTotal << std::endl;
    out << this->rotasSolucao.size() << std::endl;
    out << clocksUsados << std::endl;
    out << clocksUsados;

    for (int i = 0; i < this->rotasSolucao.size(); i++) {
        out << "\n"
            << "0 " << "1 "
            << std::setw(2) << std::right << i + 1 << " "
            << std::setw(2) << std::right << this->rotasSolucao[i].servicosAtendidos.size() << " "
            << std::setw(4) << std::right << this->rotasSolucao[i].custoTotal << " "
            << std::setw(2) << std::right << this->rotasSolucao[i].servicosPrestados.size() << " ";

        for (const auto& servico : this->rotasSolucao[i].servicosPrestados) {
            servico.imprimirServico(out);  // precisa receber o stream como argumento
        }
    }

    out << std::endl;
    out.close();
  }
};


/**
 * TODO:
 * saída de acordo com o esperado
 * atualizar readme com instruções sobre como passar arquivo via CLI
 */