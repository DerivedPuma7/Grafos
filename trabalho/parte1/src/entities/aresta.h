#pragma once

using namespace std;

#include <iostream>
#include <string>

class Aresta {
public: 
  string id;
  int origem;
  int destino;
  int peso;
  int demanda;
  int custoServico;
  Aresta(string id, int origem, int destino, int peso, int demanda, int custo) {
    this->id = id;
    this->origem = origem;
    this->destino = destino;
    this->peso = peso;
    this->demanda = demanda;
    this->custoServico = custo;
  }

  bool operator<(const Aresta& other) const {
    return this->id < other.id;
  }
};