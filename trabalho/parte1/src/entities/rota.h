#include <vector>
#include "servico.h"

class Rota {
public:
    vector<int> caminho;
    vector<Servico> servicosAtendidos;
    int custoTotal = 0;
};