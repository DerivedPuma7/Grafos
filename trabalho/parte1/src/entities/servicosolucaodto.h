
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

    void imprimirServico()
    {
        string tipoStr = (tipo == TipoServicoPrestado::D) ? "D" : "S";
        cout << "(" << setw(2) << tipoStr << ","
             << setw(2) << id << ","
             << setw(2) << from << ","
             << setw(2) << to << ") ";
    }
};