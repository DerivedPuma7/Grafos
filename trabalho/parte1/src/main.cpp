#include <iostream>
#include "entities/grafo.h"
#include "utils/fileReader.h"
#include "utils/logger.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

GraphData readInputFile(string inputFilesDir, string fileName) {
  GraphData graphData;
  logger("\n" + getCurrentDateTime("now") + " Processando arquivo: " + fileName);
  graphData.loadFromFile(inputFilesDir + fileName);
  logDataFromInputFiles(graphData);
  logger(getCurrentDateTime("now") + " Arquivo " + fileName + " processado com sucesso");
  return graphData;
}

vector<string> getInputFiles(string inputFilesDir) {
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
  cout << "fazendo progresso..." << " grafo: " << graphData.name  << "- vertices: " << graphData.totalNodes << endl;
  for (const auto& node : graphData.requiredNodesList) {
    graph.adicionarVerticeRequerido(node.id, node.demand, node.serviceCost);
  }
  cout << "vertices requeridos ok" << endl;
  for (const auto& edge : graphData.requiredEdgesList) {
    graph.adicionarAresta(edge.from, edge.to, edge.traversalCost, edge.demand, edge.serviceCost, true);
  }
  cout << "aresta requeridas ok" << endl;
  for (const auto& arc : graphData.requiredArcsList) {
    graph.adicionarArco(arc.from, arc.to, arc.traversalCost, arc.demand, arc.serviceCost, true);
  }
  cout << "arcos requeridas ok" << endl;
  for (const auto& edge : graphData.regularEdgesList) {
    graph.adicionarAresta(edge.from, edge.to, edge.traversalCost, 0, 0, false);
  }
  cout << "aresta regulares ok" << endl;
  for (const auto& arc : graphData.regularArcsList) {
    graph.adicionarArco(arc.from, arc.to, arc.traversalCost, 0, 0, false);
  }
  cout << "arcos regulares ok" << endl;
  
  return graph;
}

void registerMetrics(Grafo graph) {
  WAndPred wAndPred = graph.getWAndPred();
  graph.imprimirMatrizW(wAndPred.first);
  graph.imprimirPred(wAndPred.second);

  cout << "1- Quantidade de vértices: " << graph.getQuantidadeVertices() << endl;
  cout << "2- Quantidade de arestas: " << graph.getQuantidadeArestas() << endl;
  cout << "3- Quantidade de arcos: " << graph.getQuantidadeArcos() << endl;
  cout << "4- Quantidade de vértices requeridos: " << graph.getQuantidadeVerticesRequeridos() << endl;
  cout << "5- Quantidade de arestas requeridas: " << graph.getQuantidadeArestasRequeridas() << endl;
  cout << "6- Quantidade de arcos requeridos: " << graph.getQuantidadeArcosRequeridos() << endl;
  cout << "7- Densidade do grafo: " << graph.getDensidadeGrafo() << endl;
  cout << "8- Componentes conectados: " << graph.getComponentesConectados() << endl;
  cout << "9- Grau: " << endl;
  cout << "\t9.1- Grau mínimo de entrada: " << graph.getGrauMinEntrada() << endl;
  cout << "\t9.2- Grau máximo de entrada: " << graph.getGrauMaxEntrada() << endl;
  cout << "\t9.3- Grau mínimo de saída: " << graph.getGrauMinSaida() << endl;
  cout << "\t9.4- Grau máximo de saída: " << graph.getGrauMaxSaida() << endl;
  cout << "10- Intermediação: " << endl;
  vector<double> intermediacao = graph.getIntermediacao();
  cout << "\tIntermediação dos nós:" << endl;
  for(int i = 0; i < graph.getQuantidadeVertices(); i++) {
    cout << "\tNó " << i << ": " << intermediacao[i] << endl;
  }
  cout << "11- Caminho médio: " << graph.getCaminhoMedio() << endl;
  cout << "12- Diâmetro: " << graph.calcularDiametro() << endl;
}

int main() {
  string inputFilesDir = "../exemplos/";
  string now = getCurrentDateTime("now");
  logger("\n\n" + now + " Iniciando processamento do diretório " + inputFilesDir);

  vector<string> datFiles = getInputFiles(inputFilesDir);
  if (datFiles.empty()) {
    logger("Nenhum arquivo .dat encontrado no diretório " + inputFilesDir);
    return 1;
  }

  GraphData graphData = readInputFile(inputFilesDir, "BHW1.dat");
  Grafo grafo = registerGraph(graphData);
  grafo.floydWarshall();

  cout << "1- Quantidade de vértices: " << grafo.getQuantidadeVertices() << endl;
  cout << "2- Quantidade de arestas: " << grafo.getQuantidadeArestas() << endl;
  cout << "3- Quantidade de arcos: " << grafo.getQuantidadeArcos() << endl;
  cout << "4- Quantidade de vértices requeridos: " << grafo.getQuantidadeVerticesRequeridos() << endl;
  cout << "5- Quantidade de arestas requeridas: " << grafo.getQuantidadeArestasRequeridas() << endl;
  cout << "6- Quantidade de arcos requeridos: " << grafo.getQuantidadeArcosRequeridos() << endl;
  cout << "7- Densidade do grafo: " << grafo.getDensidadeGrafo() << endl;
  cout << "8- Componentes conectados: " << grafo.getComponentesConectados() << endl;
  cout << "9- Grau: " << endl;
  cout << "\t9.1- Grau mínimo de entrada: " << grafo.getGrauMinEntrada() << endl;
  cout << "\t9.2- Grau máximo de entrada: " << grafo.getGrauMaxEntrada() << endl;
  cout << "\t9.3- Grau mínimo de saída: " << grafo.getGrauMinSaida() << endl;
  cout << "\t9.4- Grau máximo de saída: " << grafo.getGrauMaxSaida() << endl;
  cout << "10- Intermediação: " << endl;
  vector<double> intermediacao = grafo.getIntermediacao();
  cout << "\tIntermediação dos nós:" << endl;
  for(int i = 0; i < grafo.getQuantidadeVertices(); i++) {
    cout << "\tNó " << i << ": " << intermediacao[i] << endl;
  }
  cout << "11- Caminho médio: " << grafo.getCaminhoMedio() << endl;
  cout << "12- Diâmetro: " << grafo.calcularDiametro() << endl;

  // for (const string& fileName : datFiles) {
  //   GraphData graphData = readInputFile(inputFilesDir, fileName);
  //   Grafo graph = registerGraph(graphData);
  //   // graph.floydWarshall();
  //   cout << "-----" << endl << endl;
  // }

  logger("Processamento concluído para " + to_string(datFiles.size()) + " arquivos");
  return 0;
}