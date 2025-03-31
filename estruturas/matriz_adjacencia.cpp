#include <iostream>

using namespace std;

int main() {
  int length = 3;
  int ** matriz;

  /*
  matriz (int**)   →   [ ptr_0 ] → [ 0  1  2 ]
                      [ ptr_1 ] → [ 1  2  3 ]
                      [ ptr_2 ] → [ 2  3  4 ]
  */

  
  matriz = new int*[length];
  for (int i = 0; i < length; i++) {
    matriz[i] = new int[length];
  }

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
        matriz[i][j] = i + j;
    }
  }

  cout << "Matriz:\n";
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
        cout << matriz[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}

