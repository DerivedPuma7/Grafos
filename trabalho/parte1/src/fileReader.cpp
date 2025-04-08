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
      cout << line << endl;
      if (line.empty() || line[0] == '#') continue;

      // Processar cabeçalho
      if (line.find("Name:") != string::npos) {
        name = line.substr(line.find("BHW"));
      }
      else if (line.find("Optimal value:") != string::npos) {
        optimalValue = stoi(line.substr(line.find_last_of(" \t") + 1));
      }
      // ... processar outros campos do cabeçalho similarmente

      // ReN => Required Nodes => Vértices Requeridos
      else if (line.find("ReN.") != string::npos) {
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
      else if (line.find("ReE.") != string::npos) {
        while (getline(file, line) && !line.empty()) {
          stringstream ss(line);
          RequiredEdge edge;
          ss >> edge.id >> edge.from >> edge.to >> edge.traversalCost >> edge.demand >> edge.serviceCost;
          requiredEdgesList.push_back(edge);
        }
      }

      // ReA => Required Arcs => Arcos Requeridos
      else if (line.find("ReA.") != string::npos) {
        while (getline(file, line) && !line.empty()) {
          stringstream ss(line);
          RequiredArc arc;
          ss >> arc.id >> arc.from >> arc.to >> arc.traversalCost >> arc.demand >> arc.serviceCost;
          requiredArcsList.push_back(arc);
        }
      }

      // EDGE => Arestas Regulares
      else if (line == "EDGE    FROM N.    TO N.    T. COST") {
        while (getline(file, line) && !line.empty() && line.find("ReA.") == string::npos) {
          stringstream ss(line);
          RegularEdge edge;
          ss >> edge.from >> edge.to >> edge.traversalCost;
          regularEdgesList.push_back(edge);
        }
      }

      // Arc => Arcos Regulares
      else if (line == "ARC    FROM N.    TO N.    T. COST") {
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

int main() {
  GraphData graphData;
  graphData.loadFromFile("../exemplos/BHW1.dat");

  vector<RequiredNode> requiredNodesList = graphData.requiredNodesList;
  cout << "Lista de nós requeridos:" << endl;
  for (const auto& node : requiredNodesList) {
    cout << "Nó: " << node.id << ", Demanda: " << node.demand << ", Custo de Serviço: " << node.serviceCost << endl;
  }

  vector<RequiredEdge> requiredEdgesList = graphData.requiredEdgesList;
  cout << "Lista de arestas requeridas:" << endl;
  for (const auto& edge : requiredEdgesList) {
    cout << "Aresta: " << edge.id << ", De: " << edge.from << ", Para: " << edge.to << ", Custo de Travessia: " << edge.traversalCost
      << ", Demanda: " << edge.demand << ", Custo de Serviço: " << edge.serviceCost << endl;
  }

  vector<RequiredArc> requiredArcsList = graphData.requiredArcsList;
  cout << "Lista de arcos requeridos:" << endl;
  for (const auto& arc : requiredArcsList) {
    cout << "Arco: " << arc.id << ", De: " << arc.from << ", Para: " << arc.to << ", Custo de Travessia: " << arc.traversalCost
      << ", Demanda: " << arc.demand << ", Custo de Serviço: " << arc.serviceCost << endl;
  }

  vector<RegularEdge> regularEdgesList = graphData.regularEdgesList;
  cout << "Lista de arestas regulares:" << endl;
  for (const auto& edge : regularEdgesList) {
    cout << "Aresta: De " << edge.from << ", Para: " << edge.to << ", Custo de Travessia: " << edge.traversalCost << endl;
  }

  vector<RegularArc> regularArcsList = graphData.regularArcsList;
  cout << "Lista de arcos regulares:" << endl;
  for (const auto& arc : regularArcsList) {
    cout << "Arco: " << arc.id << ", De: " << arc.from << ", Para: " << arc.to << ", Custo de Travessia: " << arc.traversalCost << endl;
  }

  return 0;
}