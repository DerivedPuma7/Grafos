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

int main() {
  string inputFilesDir = "../exemplos/";
  string now = getCurrentDateTime("now");
  vector<Grafo> grafoList;
  logger("\n\n" + now + " Iniciando processamento do diretório " + inputFilesDir);

  vector<string> datFiles = readInputDirectory(inputFilesDir);
  sort(datFiles.begin(), datFiles.end());
  if (datFiles.empty()) {
    logger("Nenhum arquivo .dat encontrado no diretório " + inputFilesDir);
    return 1;
  }
  
  for (const string& fileName : datFiles) {
    GraphData graphData = readInputFile(inputFilesDir, fileName);
    Grafo grafo = registerGraph(graphData);
    grafo.floydWarshall();
    grafoList.push_back(grafo);
  }
  logger("Processamento concluído para " + to_string(datFiles.size()) + " arquivos");
  escreverResultadosArquivoCsv(grafoList);
  cout << "Logs escritos nos arquivos do diretório logs/" << endl;
  cout << "Visualização disponível no arquivo visualizacao.ipynb" << endl;
  return 0;
}