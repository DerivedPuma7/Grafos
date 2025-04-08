#include <iostream>
#include "entities/grafo.h"
#include "entities/vertice.h"
#include "utils/fileReader.h"
#include "utils/logger.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

// int main() {
//   GraphData graphData;
//   string now = getCurrentDateTime("now");
//   string inputFilesDir = "../exemplos/";
//   string fileName = "BHW1.dat";

//   logger("\n\n" + now + " Iniciando leitura do arquivo " + fileName);
//   graphData.loadFromFile(inputFilesDir + fileName);
//   logDataFromInputFiles(graphData);

//     Grafo grafo(2);
//   return 0;
// }

int main() {
  string inputFilesDir = "../exemplos/";
  string now = getCurrentDateTime("now");
  logger("\n\n" + now + " Iniciando processamento do diretório " + inputFilesDir);

  vector<string> datFiles;

  try {
    for (const auto& entry : fs::directory_iterator(inputFilesDir)) {
      if (entry.is_regular_file() && entry.path().extension() == ".dat") {
        datFiles.push_back(entry.path().filename().string());
      }
    }
  } catch (const fs::filesystem_error& e) {
    logger("Erro ao acessar o diretório: " + string(e.what()));
    return 1;
  }

  for (const string& fileName : datFiles) {
    logger("\n" + getCurrentDateTime("now") + " Processando arquivo: " + fileName);

    GraphData graphData;
    graphData.loadFromFile(inputFilesDir + fileName);
    logDataFromInputFiles(graphData);

    Grafo grafo(graphData.totalNodes);
    logger(getCurrentDateTime("now") + " Arquivo " + fileName + " processado com sucesso");
  }

  logger("Processamento concluído para " + to_string(datFiles.size()) + " arquivos");
  return 0;
}