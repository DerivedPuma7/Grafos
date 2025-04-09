#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

string getCurrentDateTime(string s){
  time_t now = time(0);
  struct tm tstruct;
  char  buf[80];
  tstruct = *localtime(&now);
  if(s=="now")
      strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
  else if(s=="date")
      strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
  return string(buf);
};

void logger(string logMsg){
  string filePath = "../logs/log_"+getCurrentDateTime("date")+".txt";
  
  ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
  ofs << logMsg << '\n';
  ofs.close();
}

void logDataFromInputFiles(GraphData graphData) {
  vector<RequiredNode> requiredNodesList = graphData.requiredNodesList;
  logger("\tLista de nós requeridos:");
  for (const auto& node : requiredNodesList) {
    logger("\t\tNó: " + to_string(node.id) + ", Demanda: " + to_string(node.demand) + ", Custo de Serviço: " + to_string(node.serviceCost));
  }

  vector<RequiredEdge> requiredEdgesList = graphData.requiredEdgesList;
  logger("\tLista de arestas requeridas:");
  for (const auto& edge : requiredEdgesList) {
    logger("\t\tAresta: " + edge.id + ", De: " + to_string(edge.from) + ", Para: " + to_string(edge.to) +
      ", Custo de Travessia: " + to_string(edge.traversalCost) + ", Demanda: " + to_string(edge.demand) +
      ", Custo de Serviço: " + to_string(edge.serviceCost));
  }

  vector<RequiredArc> requiredArcsList = graphData.requiredArcsList;
  logger("\tLista de arcos requeridos:");
  for (const auto& arc : requiredArcsList) {
    logger("\t\tArco: " + arc.id + ", De: " + to_string(arc.from) + ", Para: " + to_string(arc.to) +
      ", Custo de Travessia: " + to_string(arc.traversalCost) + ", Demanda: " + to_string(arc.demand) +
      ", Custo de Serviço: " + to_string(arc.serviceCost));
  }

  vector<RegularEdge> regularEdgesList = graphData.regularEdgesList;
  logger("\tLista de arestas regulares:");
  for (const auto& edge : regularEdgesList) {
    logger("\t\tAresta: De " + to_string(edge.from) + ", Para: " + to_string(edge.to) +
      ", Custo de Travessia: " + to_string(edge.traversalCost));
  }

  vector<RegularArc> regularArcsList = graphData.regularArcsList;
  logger("\tLista de arcos regulares:");
  for (const auto& arc : regularArcsList) {
    logger("\t\tArco: " + arc.id + ", De: " + to_string(arc.from) + ", Para: " + to_string(arc.to) +
      ", Custo de Travessia: " + to_string(arc.traversalCost));
  }
}