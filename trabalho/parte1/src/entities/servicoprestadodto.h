
#include <iomanip>
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

enum TipoServicoPrestado {
    S,
    D
};

class ServicoPrestadoDto {

public:
    TipoServicoPrestado tipo;
    string id;
    int from;
    int to;

    ServicoPrestadoDto(TipoServicoPrestado tipo, const string& id, int from, int to)
        : tipo(tipo), id(id), from(from), to(to) {}

    void imprimirServico() const {
        string tipoStr = (tipo == TipoServicoPrestado::D) ? "D" : "S";
        cout << "(" << tipoStr << ","
             << id << ","
             << from << ","
             << to << ") ";
    }
};