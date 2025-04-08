#include <iostream>
#include "entities/grafo.h"
#include "entities/vertice.h"
#include "utils/fileReader.h"
#include "utils/logger.h"

using namespace std;

int main() {
  GraphData graphData;
  string now = getCurrentDateTime("now");
  string inputFilesDir = "../exemplos/";
  string fileName = "BHW1.dat";

  logger("\n\n" + now + " Iniciando leitura do arquivo " + fileName);
  graphData.loadFromFile(inputFilesDir + fileName);
  logDataFromInputFiles(graphData);

    Grafo grafo(2);
  return 0;
}