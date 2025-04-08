#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct RequiredNode {
  int id;
  int demand;
  int serviceCost;
};

struct RequiredEdge {
  string id;
  int from;
  int to;
  int traversalCost;
  int demand;
  int serviceCost;
};

struct RequiredArc {
  string id;
  int from;
  int to;
  int traversalCost;
  int demand;
  int serviceCost;
};

struct RegularEdge {
  int from;
  int to;
  int traversalCost;
};

struct RegularArc {
  string id;
  int from;
  int to;
  int traversalCost;
};

string normalizeString(string str) {
  // Remove múltiplos espaços/tabs
  str.erase(unique(str.begin(), str.end(), 
            [](char a, char b) { return isspace(a) && isspace(b); }), 
            str.end());
  // Converte tabs para espaços
  replace(str.begin(), str.end(), '\t', ' ');
  // Trim espaços no início/fim
  str.erase(0, str.find_first_not_of(' '));
  str.erase(str.find_last_not_of(' ') + 1);
  return str;
}

class GraphData {
public:
  string name;
  int optimalValue;
  int vehicles;
  int capacity;
  int depotNode;
  int totalNodes;
  int totalEdges;
  int totalArcs;
  int requiredNodes;
  int requiredEdges;
  int requiredArcs;
  
  vector<RequiredNode> requiredNodesList;
  vector<RequiredEdge> requiredEdgesList;
  vector<RequiredArc> requiredArcsList;
  vector<RegularEdge> regularEdgesList;
  vector<RegularArc> regularArcsList;

  void loadFromFile(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
      if(line.empty() || line[0] == '#') continue;

      // Processar cabeçalho
      if(line.find("Name:") != string::npos) {
        name = line.substr(line.find("BHW"));
      }
      else if(line.find("Optimal value:") != string::npos) {
        optimalValue = stoi(line.substr(line.find_last_of(" \t") + 1));
      }

      // ReN => Required Nodes => Vértices Requeridos
      else if(line.find("ReN.") != string::npos) {
        while (getline(file, line) && !line.empty()) {
          stringstream ss(line);
          RequiredNode node;
          string nodeId;
          ss >> nodeId >> node.demand >> node.serviceCost;
          node.id = stoi(nodeId.substr(1)); // Remove 'N' do ID
          requiredNodesList.push_back(node);
        }
      }

      // ReE => Required Edges => Arestas Requeridas
      else if(line.find("ReE.") != string::npos) {
        while (getline(file, line) && !line.empty()) {
          stringstream ss(line);
          RequiredEdge edge;
          ss >> edge.id >> edge.from >> edge.to >> edge.traversalCost >> edge.demand >> edge.serviceCost;
          requiredEdgesList.push_back(edge);
        }
      }

      // ReA => Required Arcs => Arcos Requeridos
      else if(line.find("ReA.") != string::npos) {
        while (getline(file, line) && !line.empty()) {
          stringstream ss(line);
          RequiredArc arc;
          ss >> arc.id >> arc.from >> arc.to >> arc.traversalCost >> arc.demand >> arc.serviceCost;
          requiredArcsList.push_back(arc);
        }
      }

      // EDGE => Arestas Regulares
      else if(line == "EDGE    FROM N.    TO N.    T. COST") {
        while (getline(file, line) && !line.empty() && line.find("ReA.") == string::npos) {
          stringstream ss(line);
          RegularEdge edge;
          ss >> edge.from >> edge.to >> edge.traversalCost;
          regularEdgesList.push_back(edge);
        }
      }

      // Arc => Arcos Regulares
      else if(normalizeString(line) == "ARC FROM N. TO N. T. COST") {
        while (getline(file, line) && !line.empty()) {
          stringstream ss(line);
          RegularArc arc;
          ss >> arc.id >> arc.from >> arc.to >> arc.traversalCost;
          regularArcsList.push_back(arc);
        }
      }
    }
    file.close();
  }
};

inline string getCurrentDateTime(string s){
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

inline void logger(string logMsg){
  string filePath = "../logs/log_"+getCurrentDateTime("date")+".txt";
  
  ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
  ofs << logMsg << '\n';
  ofs.close();
}

inline void logDataFromInputFiles(GraphData graphData) {
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

int main() {
  GraphData graphData;
  string now = getCurrentDateTime("now");
  string inputFilesDir = "../exemplos/";
  string fileName = "BHW1.dat";

  logger("\n\n" + now + " Iniciando leitura do arquivo " + fileName);
  graphData.loadFromFile(inputFilesDir + fileName);
  logDataFromInputFiles(graphData);


  return 0;
}