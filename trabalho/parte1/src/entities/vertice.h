#pragma once

using namespace std;

class Vertice {
public: 
  int id;
  int demanda;
  int custo;
  int idAlternativo;
  Vertice(int id, int demanda, int custo, int idAlternativo) {
    this->id = id;
    this->demanda = demanda;
    this->custo = custo;
    this->idAlternativo = idAlternativo;
  }

  bool operator<(const Vertice& other) const {
    return this->id < other.id;
  }
};