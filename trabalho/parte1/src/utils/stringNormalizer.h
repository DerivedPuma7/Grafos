#include <iostream>
#include <algorithm>

using namespace std;

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