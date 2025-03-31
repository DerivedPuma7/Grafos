#include <iostream>
#include <vector>

using namespace std;

int main() {
  int length = 3;
  vector<pair<char,int>>lista[length];
  
  /*
  lista
  ↓
  [0] → [{ 'A', 1 }, { 'B', 2 }]
  [1] → []
  [2] → []
  [3] → []
  [4] → []
  [5] → []
  [6] → []
  [7] → []
  [8] → []
  [9] → []
  */

  pair<char, int> pair_01 = {'A', 1};
  pair<char, int> pair_02 = {'B', 1};
  pair<char, int> pair_11 = {'C', 1};
  pair<char, int> pair_21 = {'D', 1};

  lista[0].push_back(pair_01);
  lista[0].push_back(pair_02);
  lista[1].push_back(pair_11);
  lista[2].push_back(pair_21);

  for (int i = 0; i < length; i++) {
    cout << "lista[" << i << "]: " << endl;
    for (const auto& p : lista[i]) {
      cout << p.first << " - " << p.second << endl;
    }
    cout << endl;
  }

  return 0;
}

