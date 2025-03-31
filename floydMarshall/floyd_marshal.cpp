#include <iostream>
#include <limits>

using namespace std;

int maiorInteiro() {
  return numeric_limits<int>::max();
}

void imprimeMatriz(int**& matriz, int length) {
  for (int i = 1; i <= length; i++) {
    for (int j = 1; j <= length; j++) {
      if (matriz[i][j] == maiorInteiro()) {
        cout << "INF ";
      } else {
        cout << matriz[i][j] << " ";
      }
    }
    cout << endl;
  }
}

int min(int a, int b) {
  return a < b ? a : b;
}

void inicializaMatriz(int**& matriz, int length) {
  matriz = new int*[length];
  for (int i = 1; i <= length; i++) {
    matriz[i] = new int[length];
  }
}

void preencheMatriz(int**& matriz) {
  matriz[1][1] = 0;
  matriz[1][2] = 3;
  matriz[1][3] = 8;
  matriz[1][4] = maiorInteiro();
  matriz[1][5] = -4;

  matriz[2][1] = maiorInteiro();
  matriz[2][2] = 0;
  matriz[2][3] = maiorInteiro();
  matriz[2][4] = 1;
  matriz[2][5] = 7;

  matriz[3][1] = maiorInteiro();
  matriz[3][2] = 4;
  matriz[3][3] = 0;
  matriz[3][4] = maiorInteiro();
  matriz[3][5] = maiorInteiro();

  matriz[4][1] = 2;
  matriz[4][2] = maiorInteiro();
  matriz[4][3] = -5;
  matriz[4][4] = 0;
  matriz[4][5] = maiorInteiro();

  matriz[5][1] = maiorInteiro();
  matriz[5][2] = maiorInteiro();
  matriz[5][3] = maiorInteiro();
  matriz[5][4] = 6;
  matriz[5][5] = 0;
}

void inicializaPred(int**& matriz, int**& pred, int length) {
  pred = new int*[length];
  for (int i = 1; i <= length; i++) {
    pred[i] = new int[length];
  }

  for (int i = 1; i <= length; i++) {
    for (int j = 1; j <= length; j++) {
      if(matriz[i][j] != maiorInteiro() && matriz[i][j] != 0) {
        pred[i][j] = i;
      } else {
        pred[i][j] = maiorInteiro();
      }
    }
  }
}

int ** floydWarshall(int** matrizOriginal, int**& pred, int length) {
  int ** matriz = matrizOriginal;
  int contador = 0;
  
  for(int k = 1; k <= length; k++) { // k representa o intermediario
    for(int i = 1; i <= length; i++) { // i e j vai caminhando e testando cada combinação a partir de um k fixado
      for(int j = 1; j <= length; j++) {
        contador++;

        if (
          matriz[i][k] != maiorInteiro() &&
          matriz[k][j] != maiorInteiro() &&
          matriz[i][j] > matriz[i][k] + matriz[k][j]
        ) {
          matriz[i][j] = matriz[i][k] + matriz[k][j];
          pred[i][j] = pred[k][j];
        }
        

        // if(
        //   matriz[i][k] != maiorInteiro() && 
        //   matriz[k][j] != maiorInteiro()
        // ) {
        //   int minimo = min(matriz[i][j], matriz[i][k] + matriz[k][j]);
        //   matriz[i][j] = minimo;
        // }
        
        // if(matriz[i][j] > matriz[i][k] + matriz[k][j]) {
        //   matriz[i][j] = matriz[i][k] + matriz[k][j];
        //   pred[i][j] = pred[k][j];
        // }
        
      }
    }
  }

  cout << "\n\tvertices: " << length << endl;
  cout << "\tn³: " <<  length*length*length << endl;
  cout << "\tcontador: " << contador << "\n" << endl;

  return matriz;
}

int main() {
  int length = 5;
  int** matriz;
  int** pred;

  inicializaMatriz(matriz, length);
  preencheMatriz(matriz);
  inicializaPred(matriz, pred, length);
  
  cout << "\nMatriz Original:\n";
  imprimeMatriz(matriz, length);

  cout << "\nPred:\n";
  imprimeMatriz(pred, length);

  int ** novaMatriz = floydWarshall(matriz, pred, length);
  
  cout << "\nNova Matriz:\n";
  imprimeMatriz(novaMatriz, length);

  cout << "\nPred atualizado:\n";
  imprimeMatriz(pred, length);

  return 0;
}

