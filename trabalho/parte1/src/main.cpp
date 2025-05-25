#include <iostream>
#include "entities/grafo.h"
#include "entities/solucao.h"
#include "utils/fileReader.h"
#include "utils/logger.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

bool arquivoExiste(string dir, string fileName) {
  fs::path caminhoCompleto = fs::path(dir) / fileName;
  return fs::exists(caminhoCompleto) && fs::is_regular_file(caminhoCompleto);
}

GraphData readInputFile(string inputFilesDir, string fileName) {
  GraphData graphData;
  if(!arquivoExiste(inputFilesDir, fileName)) {
    cerr << "Arquivo de entrada não encontrado\n";
    exit(-1);
  }
  logger("\n" + getCurrentDateTime("now") + " Lendo arquivo: " + fileName);
  graphData.loadFromFile(inputFilesDir + fileName);
  logDataFromInputFiles(graphData);
  logger(getCurrentDateTime("now") + " Arquivo " + fileName + " lido com sucesso\n\n");
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
  int id = 1;
  for (const auto& node : graphData.requiredNodesList) {
    graph.adicionarVerticeRequerido(node.id, node.demand, node.serviceCost, id);
    id++;
  }
  for (const auto& edge : graphData.requiredEdgesList) {
    graph.adicionarAresta(edge.from, edge.to, edge.traversalCost, edge.demand, edge.serviceCost, true, id);
    id++;
  }
  for (const auto& arc : graphData.requiredArcsList) {
    graph.adicionarArco(arc.from, arc.to, arc.traversalCost, arc.demand, arc.serviceCost, true, id);
    id++;
  }
  for (const auto& edge : graphData.regularEdgesList) {
    graph.adicionarAresta(edge.from, edge.to, edge.traversalCost, 0, 0, false, id);
    id++;
  }
  for (const auto& arc : graphData.regularArcsList) {
    graph.adicionarArco(arc.from, arc.to, arc.traversalCost, 0, 0, false, id);
    id++;
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

void processarArquivoUnico(string inputDir, string filename) {
  clock_t inicio = clock();

  GraphData graphData = readInputFile(inputDir, filename);
  Grafo grafo = registerGraph(graphData);
  grafo.floydWarshall();

  logger("Encontrando rotas para: " + grafo.getNome());
  Solucao solucao(grafo, graphData.capacity, graphData.depotNode);
  
  clock_t final = clock();
  int totalClocks = (final - inicio);

  solucao.imprimirRotas(totalClocks);
  
  logger("Rotas disponíveis no diretorio resultados/sol-" + grafo.getNome());
  cout << "Logs escritos nos arquivos do diretório logs/" << endl;
  cout << "Visualização disponível no arquivo visualizacao.ipynb" << endl;
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
    clock_t inicio = clock();
    
    GraphData graphData = readInputFile(inputDir, fileName);
    Grafo grafo = registerGraph(graphData);
    grafo.floydWarshall();
    grafoList.push_back(grafo);
    
    logger("Encontrando rotas para: " + grafo.getNome());
    Solucao solucao(grafo, graphData.capacity, graphData.depotNode);
    
    clock_t final = clock();
    int totalClocks = (final - inicio);
    
    solucao.imprimirRotas(totalClocks);
    logger("Rotas disponíveis no diretorio: resultados/sol-" + grafo.getNome());
  }
  logger("\nProcessamento concluído para " + to_string(datFiles.size()) + " arquivos");
  escreverResultadosArquivoCsv(grafoList);
  cout << "Logs escritos nos arquivos do diretório logs/" << endl;
  cout << "Visualização disponível no arquivo visualizacao.ipynb" << endl;
}

int main(int argc, char* argv[]) {
  cout << "Uso: " << argv[0] << " nome_do_arquivo.dat\n";
  cout << "ou\n";
  cout << "Uso: " << argv[0] << "\n";
  string inputFilesDir = "../exemplos/";

  if(argc == 2) {
    string filename = argv[1];
    cout << "\n\tProcessando arquivo. Aguarde...\n";
    processarArquivoUnico(inputFilesDir, filename);
  }
  if (argc < 2) {
    cout << "\n\tProcessando diretório de entrada. Aguarde...\n";
    processarDiretorioDeEntrada(inputFilesDir);
  }
  return 0;
}