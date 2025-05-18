#include <vector>
#include "servico.h"
#include "servicoprestadodto.h"

class Rota {
public:
    vector<int> caminho;
    vector<Servico> servicosAtendidos;
    vector<ServicoPrestadoDto> servicosPrestados;
    int custoTotal = 0;
};