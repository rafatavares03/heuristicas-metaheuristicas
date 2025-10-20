#include <bits/stdc++.h>
#include <chrono>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;
using namespace std::chrono;

vector<vector<int>> grafo;
vector<vector<int>> coordenadas;

int distancia(int i, int j) {
  int xd = coordenadas[i][1] - coordenadas[j][1]; 
  int yd = coordenadas[i][2] - coordenadas[j][2]; 
  return (int)(sqrt(xd*xd + yd*yd) + 0.5);
}

vector<vector<int>> matriz() {
  vector<vector<int>> g(coordenadas.size(), vector<int>(coordenadas.size(), -1));
  for(int i = 0; i < coordenadas.size(); i++) g[i][i] = 0;
  for(int i = 0; i < coordenadas.size(); i++) {
    for(int j = i+1; j < coordenadas.size(); j++){
      int peso = distancia(i, j);
      g[i][j] = peso;
      g[j][i] = peso;
    }
  }
  return g;
}

void lerEntrada() {
  int v, x, y;
  while(cin >> v >> x >> y) {
    coordenadas.push_back({v,x,y});
  }
  grafo = matriz();
}

void removeElemento(vector<int> &array, int elemento) {
  auto fim = remove(array.begin(), array.end(), elemento);
  array.erase(fim, array.end());
}

vector<int> solucaoInicial() {
  vector<int> vertices, caminho;
  for(int i = 1; i < grafo.size(); i++) vertices.push_back(i);
  caminho.push_back(0);
  while(!vertices.empty()) {
    int v = caminho[caminho.size() - 1];
    int u, custo = INF;
    for(int i = 0; i < vertices.size(); i++) {
      if(grafo[v][vertices[i]] < custo) {
        u = vertices[i];
        custo = grafo[v][vertices[i]];
      }
    }
    caminho.push_back(u);
    removeElemento(vertices, u);
  }
  return caminho;
}

int calculaCusto(vector<int> caminho) {
  if(caminho.empty()) return 0;
  int custo = 0;
  for(int i = 0; i < caminho.size()-1; i++) {
    custo += grafo[caminho[i]][caminho[i+1]];
  }
  custo += grafo[caminho[0]][caminho[caminho.size()-1]];
  return custo;
}

void printSolucaoCSV(vector<int> solucao, duration<double> tempo) {
  for(int i = 0; i < solucao.size(); i++) {
    cout << solucao[i]+1 << " ";
  }
  cout << solucao[0]+1 << "," << calculaCusto(solucao) << ",";
  cout << fixed << setprecision(3);
  cout << tempo.count() << endl;
}

void printSolucao(vector<int> solucao, duration<double> tempo) {
  cout << "Solução: ";
  for(int i = 0; i < solucao.size(); i++) cout << solucao[i] << " ";
  cout << solucao[0] << endl;
  cout << "Custo: " << calculaCusto(solucao) << endl;
  cout << fixed << setprecision(3);
  cout << tempo.count() << endl;
}

vector<int> swap(vector<int> array, int i, int j) {
  vector<int> aux(array);
  int x = aux[i];
  aux[i] = aux[j];
  aux[j] = x;
  return aux;
}

vector<vector<int>> geraVizinhancaOrdenada(vector<int> solucao) {
  vector<vector<int>> vizinhos;
  for(int i = 0; i < grafo.size(); i++) {
    for(int j = i+1; j < grafo.size(); j++) {
      int c = calculaCusto(swap(solucao, i, j));
      vizinhos.push_back({i,j,c});
    }
  }
  sort(vizinhos.begin(), vizinhos.end(), [](vector<int> &a, vector<int> &b) {
    return a[2] < b[2];
  });

  return vizinhos;
}

vector<int> buscaTabu() {
  vector<int> solucao = solucaoInicial();
  vector<int> melhorSolucao = solucao;
  int melhorCusto = calculaCusto(solucao);

  vector<vector<int>> t(grafo.size(), vector<int>(grafo.size(), 0));
  int duracaoTabu = max(1, (int)(0.6 * grafo.size()));
  int it = 0;
  while(it < 1e3) {
    vector<vector<int>> vizinhos = geraVizinhancaOrdenada(solucao);
    vector<int> melhorTroca;
    int custo = INF;
    int k = 0;
    int u, v;
    while(melhorTroca.empty() && k < vizinhos.size()) {
      u = vizinhos[k][0];
      v = vizinhos[k][1];
      int novoCusto = vizinhos[k][2];
      if(t[min(u,v)][max(u,v)] > 0) {
        if(novoCusto < melhorCusto) {
          melhorTroca = swap(solucao, u, v);
          custo = novoCusto;
        }
      } else if(novoCusto < custo) {
          melhorTroca = swap(solucao, u, v);
          custo = novoCusto;
        }
      k++;
    }
    //if(melhorTroca.empty()) cout << tem que inspirar
    for(int i = 0; i < t.size(); i++) {
      for(int j = i+1; j < t.size(); j++) {
        if(t[i][j] > 0) t[i][j]--;
      }
    }
    t[min(u,v)][max(u,v)] = duracaoTabu;
    solucao = melhorTroca;
    if(custo < melhorCusto) {
      melhorSolucao = solucao;
      melhorCusto = custo;
    }
    it++;
  }

  return melhorSolucao;
}

int main() {
  lerEntrada();
  auto inicio = high_resolution_clock::now();
  vector<int> solucao = buscaTabu();
  auto fim = high_resolution_clock::now();
  duration<double> tempo = fim - inicio;
  printSolucaoCSV(solucao, tempo);
  return 0;
}