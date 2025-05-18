#include <iostream>
#include "entities/grafo.h"
#include "entities/solucao.h"
#include "utils/fileReader.h"
#include "utils/logger.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

bool arquivoExiste(string dir, string filename) {
  fs::path caminhoCompleto = fs::path(dir) / filename;
  return fs::exists(caminhoCompleto) && fs::is_regular_file(caminhoCompleto);
}

GraphData readInputFile(string inputFilesDir, string fileName) {
  GraphData graphData;
  logger("\n" + getCurrentDateTime("now") + " Processando arquivo: " + fileName);
  graphData.loadFromFile(inputFilesDir + fileName);
  logDataFromInputFiles(graphData);
  logger(getCurrentDateTime("now") + " Arquivo " + fileName + " processado com sucesso\n\n");
  return graphData;
}

vector<string> readInputDirectory(string inputFilesDir) {
  vector<string> datFiles;
  try {
    for (const auto& entry : fs::directory_iterator(inputFilesDir)) {
      if (entry.is_regular_file() && entry.path().extension() == ".dat") {
        datFiles.push_back(entry.path().filename().string());
      }
    }
    return datFiles;
  } catch (const fs::filesystem_error& e) {
    logger("Erro ao acessar o diretório: " + string(e.what()));
    return datFiles;
  }
}

Grafo registerGraph(GraphData graphData) {
  Grafo graph(graphData.totalNodes, graphData.name);
  for (const auto& node : graphData.requiredNodesList) {
    graph.adicionarVerticeRequerido(node.id, node.demand, node.serviceCost);
  }
  for (const auto& edge : graphData.requiredEdgesList) {
    graph.adicionarAresta(edge.from, edge.to, edge.traversalCost, edge.demand, edge.serviceCost, true);
  }
  for (const auto& arc : graphData.requiredArcsList) {
    graph.adicionarArco(arc.from, arc.to, arc.traversalCost, arc.demand, arc.serviceCost, true);
  }
  for (const auto& edge : graphData.regularEdgesList) {
    graph.adicionarAresta(edge.from, edge.to, edge.traversalCost, 0, 0, false);
  }
  for (const auto& arc : graphData.regularArcsList) {
    graph.adicionarArco(arc.from, arc.to, arc.traversalCost, 0, 0, false);
  }
  return graph;
}

void escreverMetricasGerais(vector<Grafo> graphList) {
  try {
    string filename = "metricasGerais.csv";
    cout << "Escrevendo métricas gerais no arquivo: " << filename << endl;
    ofstream file(filename);
    file << "NOME,QTD_VERTICES,QTD_ARESTAS,QTD_ARCOS,QTD_VERTICES_REQUERIDOS,QTD_ARESTAS_REQUERIDAS,QTD_ARCOS_REQUERIDOS,DENSIDADE_GRAFO,GRAU_MINIMO_ENTRADA,GRAU_MAXIMO_ENTRADA,GRAU_MINIMO_SAIDA,GRAU_MAXIMO_SAIDA,CAMINHO_MEDIO,DIAMETRO" << endl;
    for (auto graph : graphList) {
      file 
        << graph.getNome() << ","
        << graph.getQuantidadeVertices() << ","
        << graph.getQuantidadeArestas() << ","
        << graph.getQuantidadeArcos() << ","
        << graph.getQuantidadeVerticesRequeridos() << ","
        << graph.getQuantidadeArestasRequeridas() << ","
        << graph.getQuantidadeArcosRequeridos() << ","
        << graph.getDensidadeGrafo() << ","
        << graph.getGrauMinEntrada() << ","
        << graph.getGrauMaxEntrada() << ","
        << graph.getGrauMinSaida() << ","
        << graph.getGrauMaxSaida() << ","
        << graph.getCaminhoMedio() << ","
        << graph.getDiametro()
        << endl;
    }
    file.close();
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void escreverIntermediacoes(vector<Grafo> graphList) {
  try {
    string filename = "intermediacoes.csv";
    cout << "Escrevendo intermediações no arquivo: " << filename << endl;
    
    ofstream file(filename);
    file << "NOME, VERTICE, INTERMEDIACAO" << endl;
    
    for(auto graph : graphList) {
      vector<double> intermediacao = graph.getIntermediacao();
      for(int i = 1; i <= graph.getQuantidadeVertices(); i++) {
        file 
          << graph.getNome() << ","
          << i << ","
          << intermediacao[i]
          << endl;
      }
    }
    file.close();
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void escreverResultadosArquivoCsv(vector<Grafo> graphList) {
  escreverMetricasGerais(graphList);
  escreverIntermediacoes(graphList);
}

// int main() {
//   string inputFilesDir = "../exemplos/";
//   string now = getCurrentDateTime("now");
//   vector<Grafo> grafoList;
//   logger("\n\n" + now + " Iniciando processamento do diretório " + inputFilesDir);

//   vector<string> datFiles = readInputDirectory(inputFilesDir);
//   sort(datFiles.begin(), datFiles.end());
//   if (datFiles.empty()) {
//     logger("Nenhum arquivo .dat encontrado no diretório " + inputFilesDir);
//     return 1;
//   }
  
//   for (const string& fileName : datFiles) {
//     GraphData graphData = readInputFile(inputFilesDir, fileName);
//     Grafo grafo = registerGraph(graphData);
//     grafo.floydWarshall();
//     grafoList.push_back(grafo);
//     break;
//   }
//   logger("Processamento concluído para " + to_string(datFiles.size()) + " arquivos");
//   escreverResultadosArquivoCsv(grafoList);
//   cout << "Logs escritos nos arquivos do diretório logs/" << endl;
//   cout << "Visualização disponível no arquivo visualizacao.ipynb" << endl;
//   return 0;
// }

void processarArquivoUnico(string inputDir, string filename) {
  GraphData graphData = readInputFile(inputDir, "BHW1.dat");
  Grafo grafo = registerGraph(graphData);
  grafo.floydWarshall();
  Solucao solucao(grafo, graphData.capacity, graphData.depotNode);
}

void processarDiretorioDeEntrada(string inputDir) {
  vector<string> datFiles = readInputDirectory(inputDir);
  vector<Grafo> grafoList;
  sort(datFiles.begin(), datFiles.end());
  if (datFiles.empty()) {
    logger("Nenhum arquivo .dat encontrado no diretório " + inputDir);
    return;
  }
  
  for (const string& fileName : datFiles) {
    GraphData graphData = readInputFile(inputDir, fileName);
    Grafo grafo = registerGraph(graphData);
    grafo.floydWarshall();
    grafoList.push_back(grafo);
    Solucao solucao(grafo, graphData.capacity, graphData.depotNode);
    break;
  }
  logger("Processamento concluído para " + to_string(datFiles.size()) + " arquivos");
  escreverResultadosArquivoCsv(grafoList);
  cout << "Logs escritos nos arquivos do diretório logs/" << endl;
  cout << "Visualização disponível no arquivo visualizacao.ipynb" << endl;
}

int main(int argc, char* argv[]) {
  cout << "Uso: " << argv[0] << " <nome_do_arquivo>\n";
  cout << "ou\n";
  cout << "Uso: " << argv[0] << "\n";
  string inputFilesDir = "../exemplos/";

  cout << "argc: " << argc << endl;
  cout << "argv[0]: " << argv[0] << endl;
  cout << "argv[1]: " << argv[1] << endl << endl;

  if(argc == 2) {
    string nomeArquivo = argv[1];
    processarArquivoUnico(inputFilesDir, nomeArquivo);
  }
  if (argc < 2) {
    processarDiretorioDeEntrada(inputFilesDir);
  }
  return 0;

  // string now = getCurrentDateTime("now");
  // vector<Grafo> grafoList;
  // logger("\n\n" + now + " Iniciando processamento do diretório " + inputFilesDir);

  // vector<string> datFiles = readInputDirectory(inputFilesDir);
  // sort(datFiles.begin(), datFiles.end());
  // if (datFiles.empty()) {
  //   logger("Nenhum arquivo .dat encontrado no diretório " + inputFilesDir);
  //   return 1;
  // }
  
  // GraphData graphData = readInputFile(inputFilesDir, "BHW1.dat");
  // Grafo grafo = registerGraph(graphData);
  // grafo.floydWarshall();
  // grafoList.push_back(grafo);

  // Solucao solucao(grafo, graphData.capacity, graphData.depotNode);

  // grafo.imprimirVerticesRequeridos();
  // grafo.imprimirArestasRequeridas();
  // grafo.imprimirArcosRequeridos();

  // logger("Processamento concluído para " + to_string(datFiles.size()) + " arquivos");
  // escreverResultadosArquivoCsv(grafoList);
  // cout << "Logs escritos nos arquivos do diretório logs/" << endl;
  // cout << "Visualização disponível no arquivo visualizacao.ipynb" << endl;
  return 0;
}