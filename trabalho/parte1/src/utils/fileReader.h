#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../utils/stringNormalizer.h"

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


