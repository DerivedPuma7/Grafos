#include<iostream>
#include<vector>
#include<utility>
using namespace std;

typedef vector<int> vi;
typedef pair<int,int> ii;

// Implemente sua resposta aqui
ii retornaGrau(vi* LA, int n, int u)
{
  // first: grau de entrada; second: grau de saida
  ii grau;
  
  grau.second = LA[u].size();
  grau.first = 0;

  for(int i = 0; i < n; i++){
    for(int j = 0; j < LA[i].size(); j++) {
      if(LA[i][j] == u) {
        grau.first++;
      }
    }
  }

  return grau;
}

void inicializaListaAdjacencia(vi* &LA) {
  LA[0].push_back(1);
  LA[0].push_back(2);
  LA[0].push_back(4);

  LA[1].push_back(4);
  LA[1].push_back(3);

  LA[2].push_back(1);

  LA[3].push_back(2);
  LA[3].push_back(0);

  LA[4].push_back(3);
}

void imprimeListaAdjacencia(vi* LA, int n) {
  for(int i = 0; i < n; i++) {
    cout << "vertice " << i << ": ";
    for(int j = 0; j < LA[i].size(); j++)
      cout << LA[i][j] << " ";
    cout << endl;
  }
}

void imprimeCaminhosDoVertice(vi* LA, int vertice) {
  for (int i = 0; i < LA[vertice].size(); i++) {
    cout << "caminho do vertice " << vertice << " para o vertice " << LA[vertice][i] << endl;
  }
  cout << endl;
}


int main()
{
  // n = numero de vertices, m = numero de arestas
  int n, m;

  cout << "Insira o valor de \"n\" e \"m\" " << endl;
  cin >> n >> m;

  int parar;

  while(!cin.eof())
  {
    // considerando que os índices dos vértices começam de 0 e vão até n-1
    vi* LA = new vi[n];
    inicializaListaAdjacencia(LA);
    imprimeListaAdjacencia(LA, n);
    imprimeCaminhosDoVertice(LA, 0);

    // leitura do grafo
    // int u, v;
    // for(int i = 0; i < m; i++)
    // {
    //   cout << "Insira o valor de \"u\" e \"v\" " << endl;
    //   cin >> u >> v; // lendo as arestas do grafo

    //   // grafo direcionado
    //   LA[u].push_back(v); //u -> v
    // }

    // escolhe aleatoriamente um vertice de 0 a n-1

    int vertice;
    cout << "escolha o vertice entre 1 e " <<  n << endl;
    cin >> vertice;
    ii grau = retornaGrau(LA, n, vertice);
    cout << "vertice: " << vertice << " " << "grau.first: " << grau.first << " " << "grau.second: " << grau.second << endl;

    cout << "deseja parar? [0 - sim, 1 - nao]" << endl;
    cin >> parar;
    if(parar == 0) {
      break;
    }
  }

  return 0;
}