#pragma once

using namespace std;

class Vertice {
public: 
  int id;
  int demanda;
  int custo;
  Vertice(int id, int demanda, int custo) {
    this->id = id;
    this->demanda = demanda;
    this->custo = custo;
  }

  bool operator<(const Vertice& other) const {
    return this->id < other.id;
  }
};