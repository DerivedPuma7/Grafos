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
  int idAlternativo;
  Aresta(string id, int origem, int destino, int peso, int demanda, int custo, int idAlternativo) {
    this->id = id;
    this->origem = origem;
    this->destino = destino;
    this->peso = peso;
    this->demanda = demanda;
    this->custoServico = custo;
    this->idAlternativo = idAlternativo;
  }

  bool operator<(const Aresta& other) const {
    return this->id < other.id;
  }
};