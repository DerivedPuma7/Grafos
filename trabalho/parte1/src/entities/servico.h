#pragma once

#include <iostream>

using namespace std;

enum TipoServico {
    NO,
    ARESTA,
    ARCO
};

class Servico {
public:
    TipoServico tipo;
    string id;
    int from;
    int to;
    int demanda;
    int custoServico;
    bool atendido = false;
};