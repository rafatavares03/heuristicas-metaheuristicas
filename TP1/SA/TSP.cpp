#include <bits/stdc++.h>
#define endl '\n'
#define INF 0x3f3f3f3f

using namespace std;

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

int calculaCusto(vector<int> caminho) {
  if(caminho.empty()) return 0;
  int custo = 0;
  for(int i = 0; i < caminho.size()-1; i++) {
    custo += grafo[caminho[i]][caminho[i+1]];
  }
  custo += grafo[caminho[0]][caminho[caminho.size()-1]];
  return custo;
}

void printSolucao(vector<int> solucao) {
  cout << "Solução: ";
  for(int i = 0; i < solucao.size(); i++) cout << solucao[i] << " ";
  cout << solucao[0] << endl;
  cout << "Custo: " << calculaCusto(solucao) << endl;
}

vector<int> solucaoInicial() {
  vector<int> vertices, caminho;
  for(int i = 0; i < grafo.size(); i++) vertices.push_back(i);
  srand(time(0));
  while(!vertices.empty()) {
    int v = rand() % vertices.size();
    caminho.push_back(vertices[v]);
    auto fim = remove(vertices.begin(), vertices.end(), vertices[v]);
    vertices.erase(fim, vertices.end());
  }
  return caminho;
}

vector<int> swap(vector<int> array, int i, int j) {
  vector<int> aux(array);
  int x = aux[i];
  aux[i] = aux[j];
  aux[j] = x;
  return aux;
}

double random(double lowerbound, double upperbound) {
  mt19937 eng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  uniform_real_distribution<double> distrib(lowerbound, upperbound);

  double numero = distrib(eng);
  return numero;
}

vector<int> simulatedAnnealing() {
  vector<int> solucao = solucaoInicial();
  vector<int> melhorSolucao(solucao);
  double t = 1e5;
  while(t > 0.01) {
    int i = 0;
    while(i < 100) {
      int v = (int)random(0, solucao.size());
      int u = (int)random(0, solucao.size());
      if(v == u) v = (v+1) % solucao.size();
      vector<int> aux = swap(solucao, u, v);
      int custoAux = calculaCusto(aux);
      int custoSol = calculaCusto(solucao);  
      if(custoAux < custoSol) {
        solucao = aux;
        if(custoAux < calculaCusto(melhorSolucao)) {
          melhorSolucao = solucao;
        }
      } else if (random(0, 1) < exp((custoSol - custoAux)/t)) {
        solucao = aux;
      }

      i++;
    }
    t = 0.90 * t;
  }
  return melhorSolucao;
}

int main() {
  lerEntrada();
  vector<int> solucao = simulatedAnnealing();
  printSolucao(solucao);
  return 0;
}
